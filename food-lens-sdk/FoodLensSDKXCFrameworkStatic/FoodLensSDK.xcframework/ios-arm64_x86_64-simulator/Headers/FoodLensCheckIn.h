//
//  FoodLensCheckIn.h
//  FoodLensSDK
//
//  Created on 10/16/19.
//  Copyright © 2019 Azumio Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "FoodLensObject.h"

@protocol FoodLensFoodLog;
@protocol FoodLensFoodSegment;

/**
    FoodLensCheckIn represents single abstract timed user activity. Is characterized by date and unique identifier.
    Currently the only concrete activity is a food which is represented by @c FoodLensFoodCheckIn protocol below
 */
NS_SWIFT_NAME(FoodLensCheckIn)
@protocol FoodLensCheckIn <FoodLensObject, NSCoding>
/**
    The date what the checkin was created on
 */
@property (nonnull, nonatomic, readonly) NSDate *date;
/**
    The timezone in which checkin was created.
    
    Must be considered during returning on-day checkins from -[FoodLensDataSource foodLensFoodCheckIns:forDayOnDate:]
 */
@property (nonnull, nonatomic, readonly) NSTimeZone *timeZone;
@end


/**
    FoodLensCheckIn represents single food photo log
 */
NS_SWIFT_NAME(FoodLensFoodCheckIn)
@protocol FoodLensFoodCheckIn <FoodLensCheckIn>
/**
    The number of total calories
 */
@property (nonatomic, readonly) double calories;
/**
    The image URL string. Either remote or local
 */
@property (nonnull, nonatomic, readonly) NSString *imageURL;
/**
    All the food items present on the photo and confirmed by user
    One food log may belong to different food items on the photo. Check -[FoodLensFoodLog segmentCheckIns]
 */
@property (nonnull, nonatomic, readonly) NSArray<id<FoodLensFoodLog>> *foodLogs;
/**
    All the food items recognized on the photo. So called segments.
    Every segment contains a list of food variants which are offered to user for confirmation.
    Confirmed foods then are stored like @c FoodLensFoodLog in @c foodLogs.
 */
@property (nonnull, nonatomic, readonly) NSArray<id<FoodLensFoodSegment>> *foodSegments;
@end

