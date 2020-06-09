//
//  UINavigationController+FoodLens.h
//  FoodLensSDK
//
//  Created by Vyacheslav Kuleshov on 4/30/20.
//  Copyright © 2020 Azumio Inc. All rights reserved.
//

#import <UIKit/UIKit.h>

/**
    Convenient extensions to UINavigationController
 */
@interface UINavigationController (FoodLens)
/**
    Returns UINavigationController instance which is pre configured to put instances of food search view controller inside.
    
    It's recommended to put the result of @c -[FoodLens instantiateFoodSearchViewController] inside of the navigation controller for better UX.

    @code
    let viewController = foodLens.instantiateFoodSearchViewController()
    let navigationController = UINavigationController.foodNavigationController(withRootViewController: viewController)
    navigationController.modalPresentationStyle = .fullScreen
    window?.rootViewController?.present(navigationController, animated: true) {}
    @endcode

    @param rootViewController A root view controller for the resulting navigation controller
 */
+ (nonnull instancetype)foodNavigationControllerWithRootViewController:(nonnull UIViewController *)rootViewController;
@end
