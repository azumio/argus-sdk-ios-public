//
//  FoodLensFoodLog.h
//  FoodLensSDK
//
//  Created by Vyacheslav Kuleshov on 5/15/20.
//  Copyright © 2020 Azumio Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "FoodLensObject.h"

@protocol FoodLensFood;
@protocol FoodLensFoodSuggestion;
@protocol FoodLensFoodSegment;

/**
    FoodLensFoodSegmentCheckIn is a reference to food segments which @c FoodLensFoodLog belongs to
 */
@protocol FoodLensFoodSegmentCheckIn <FoodLensObject>
/**
    The segment which the checkin was created for
 */
@property (nonnull, nonatomic, readonly) id<FoodLensFoodSegment> foodSegment;
/**
    The score of the confirmed food item among other suggested for this segment.
    The score values only make sense withing one given segment.
 */
@property (nonatomic, readonly) double score;
@end


/**
    FoodLensFoodLog is a food item confirmed by user with selected serving, number of servings and calories
 */
@protocol FoodLensFoodLog <FoodLensObject>
/**
    The selected serving from one provided by @c FoodLensFood
 */
@property (nullable, nonatomic, readonly) id serving;
/**
    The number of servings
 */
@property (nonatomic, readonly) double numberOfServings;
/**
    The total number of calories in this item
 */
@property (nonatomic, readonly) double calories;

/**
    The related food identifier
 */
@property (nonnull, nonatomic, readonly) NSString *foodIdentifier;
/**
    The related food.
 */
@property (nullable, nonatomic, readonly) id<FoodLensFood> food;

/**
    The initial confirmed food suggestion.
 */
@property (nullable, nonatomic, readonly) id<FoodLensFoodSuggestion> suggestion;
/**
    The list of segment checkIns which this food item was confirmed for
    The same food may be present several times on one photo. That's why the list may contains more than one item.
 */
@property (nullable, nonatomic, readonly) NSArray<id<FoodLensFoodSegmentCheckIn>> *segmentCheckIns;
@end
