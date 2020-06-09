//
//  FoodLensFood.h
//  FoodLensSDK
//
//  Created by Vyacheslav Kuleshov on 5/15/20.
//  Copyright © 2020 Azumio Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "FoodLensObject.h"

/**
    FoodLensFood represents a food instnce with a name and available serving sizes.
    User confirms food by selecting what serving and the number of servings and then it's stores like @c FoodLensFoodLog
 */
@protocol FoodLensFood <FoodLensObject>
/**
    The identifier which unique identifies food in database
 */
@property (nonnull, nonatomic, readonly) NSString *identifier;
/**
    The human readable food name
 */
@property (nonnull, nonatomic, readonly) NSString *name;
/**
    The serving variants like an opaque objects
 */
@property (nonnull, nonatomic, readonly) NSArray *servings;
@end
