//
//  FoodLensObject.h
//  FoodLensSDK
//
//  Created by Vyacheslav Kuleshov on 5/15/20.
//  Copyright © 2020 Azumio Inc. All rights reserved.
//

/**
    Common protocol which all model FoodLens classes conforms to
 */
@protocol FoodLensObject <NSObject, NSCoding>
/**
    The model object as a dictionary.
    
    Users shouldn't rely much on the dictionary content and layout because it may change in future.
    The dictionary may reveal some internal object details
 */
@property (nonnull, nonatomic, readonly) NSDictionary *dictionaryRepresentation;
@end
