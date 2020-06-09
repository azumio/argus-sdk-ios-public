//
//  FoodLensFoodSegment.h
//  FoodLensSDK
//
//  Created by Vyacheslav Kuleshov on 5/15/20.
//  Copyright © 2020 Azumio Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "FoodLensObject.h"

@protocol FoodLensFoodCategory;
@protocol FoodLensFoodSuggestion;
@protocol FoodLensFood;

/**
    FoodLensFoodSegment represents one food item recognized on photo.
    When food photo is sent for recognition then there are several food items potentially can be found on the photo. So called segments.
    All food items per segment are grouped by categories joining similiar foods.
 */
@protocol FoodLensFoodSegment <FoodLensObject>
/**
    The categories of food items joining similiar foods.
 */
@property (nonnull, nonatomic, readonly) NSArray<id<FoodLensFoodCategory>> *categories;
@end

/**
    The category is a set similiar food items.
 */
@protocol FoodLensFoodCategory <FoodLensObject>
/**
    The suggestions. One for each of recognized food items.
    The suggestions are ordered by the score. Most likely is first.
 */
@property (nonnull, nonatomic, readonly) NSArray<id<FoodLensFoodSuggestion>> *suggestions;
@end

/**
    The suggestion is a guess about what food item it could be on the photo.
    The score reflects how likely exactly this food is on the photo.
    The score is only makes sense within one segment.
 */
@protocol FoodLensFoodSuggestion <FoodLensObject>
/**
    The food which is likely on the photo
 */
@property (nullable, nonatomic, readonly) id<FoodLensFood> food;
/**
    The score. Is a some sort of not weighted probablility.
    Only makes sense within one segment.
 */
@property (nonatomic, readonly) double score;
@end
