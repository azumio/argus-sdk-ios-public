#ifdef __OBJC__
#import <UIKit/UIKit.h>
#else
#ifndef FOUNDATION_EXPORT
#if defined(__cplusplus)
#define FOUNDATION_EXPORT extern "C"
#else
#define FOUNDATION_EXPORT extern
#endif
#endif
#endif

#import "FoodLens.h"
#import "FoodLensDataSource.h"
#import "FoodLensCheckIn.h"
#import "FoodLensFood.h"
#import "FoodLensFoodLog.h"
#import "FoodLensFoodSegment.h"
#import "FoodLensObject.h"
#import "UINavigationController+FoodLens.h"
#import "FoodLensSDK.h"

FOUNDATION_EXPORT double FoodLensSDKVersionNumber;
FOUNDATION_EXPORT const unsigned char FoodLensSDKVersionString[];

