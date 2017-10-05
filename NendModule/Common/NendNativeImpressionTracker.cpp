//
//  NendNativeImpressionTracker.cpp
//
//  Created by F@N Communications, Inc.
//
//

#include "NendNativeAdLog.h"
#include "NendNativeImpressionTracker.h"

using namespace nend_module;
using namespace nend_module::internal;

#define NAD_NATIVE_PERCENT_VISIBLE 50

static NendNativeImpressionTracker *s_sharedTracker = nullptr;
Scheduler *m_impressionTrackingScheduler;
std::map<Node *, NendNativeAd *> trackingNodeMap;

NendNativeImpressionTracker::NendNativeImpressionTracker()
{
}

NendNativeImpressionTracker *NendNativeImpressionTracker::getInstance()
{
    if (!s_sharedTracker) {
        s_sharedTracker = new (std::nothrow) NendNativeImpressionTracker();
        CCASSERT(s_sharedTracker, "FATAL: Not enough memory");
    }

    return s_sharedTracker;
}

void NendNativeImpressionTracker::addTrackingNode(NendNativeAd *nativeAd, Node *node)
{
    node->retain();
    trackingNodeMap.insert(std::make_pair(node, nativeAd));

    if (!m_impressionTrackingScheduler) {
        m_impressionTrackingScheduler = Director::getInstance()->getScheduler();
    }

    if (!m_impressionTrackingScheduler->isScheduled(CC_SCHEDULE_SELECTOR(NendNativeImpressionTracker::checkAdImpression), this)) {
        m_impressionTrackingScheduler->schedule(CC_SCHEDULE_SELECTOR(NendNativeImpressionTracker::checkAdImpression), this, 1.f, CC_REPEAT_FOREVER, 0, false);
    }
}

void NendNativeImpressionTracker::removeTrackingNode(NendNativeAd *nativeAd)
{
    auto itr = trackingNodeMap.begin();
    while (itr != trackingNodeMap.end()) {
        NendNativeAd *checkAd = (*itr).second;
        if (checkAd == nativeAd) {
            auto removeItr = itr;
            itr++;
            (*removeItr).first->release();
            trackingNodeMap.erase(removeItr);
        } else {
            itr++;
        }
    }

    if (m_impressionTrackingScheduler && 0 == trackingNodeMap.size()) {
        m_impressionTrackingScheduler->unschedule(CC_SCHEDULE_SELECTOR(NendNativeImpressionTracker::checkAdImpression), this);
    }
}

void NendNativeImpressionTracker::checkAdImpression(float dt)
{
    NendNativeAdLog::logDebug("Tracking impression...");

    auto itr = trackingNodeMap.begin();
    while (itr != trackingNodeMap.end()) {
        Node *checkNode = (*itr).first;
        if (this->isImpressionNode(checkNode)) {
            NendNativeAd *nativeAd = (*itr).second;
            nativeAd->setDidImpression(true);
            nativeAd->onImpression();
            NendNativeAdLog::logDebug("AD impression.");
            this->removeTrackingNode(nativeAd);
            itr = trackingNodeMap.begin();
        } else {
            itr++;
        }
    }
}

bool NendNativeImpressionTracker::isImpressionNode(Node *checkNode)
{
    auto runningSceneChildren = Director::getInstance()->getRunningScene()->getChildren();
    if (1 < runningSceneChildren.size()) {
        auto scene = Director::getInstance()->getRunningScene()->getChildren().at(1);
        Node *parentNode = nullptr;
        for (parentNode = checkNode; parentNode->getParent() != nullptr; parentNode = parentNode->getParent()) {
            if (parentNode == scene) {
                break;
            }
        }
        if (parentNode != scene) {
            return false;
        }

        auto sceneRect = scene->getBoundingBox();
        auto checkRect = checkNode->getBoundingBox();

        Vec2 startPoint = checkNode->convertToWorldSpace(Point::ZERO);
        Vec2 endPoint = checkNode->convertToWorldSpace(Vec2(checkRect.size.width, checkRect.size.height));

        Size displayedSize = Size(MAX(0, MIN(sceneRect.size.width, endPoint.x) - MAX(0, startPoint.x)), MAX(0, MIN(sceneRect.size.height, endPoint.y) - MAX(0, startPoint.y)));

        float intersectionArea = displayedSize.width * displayedSize.height;
        float originalArea = checkRect.size.width * checkRect.size.height;
        return intersectionArea >= (originalArea * NAD_NATIVE_PERCENT_VISIBLE / 100);
    }
    return false;
}
