//
//  SceneDelegate.swift
//  FoodLensSDKExample
//
//  Created by Vyacheslav Kuleshov on 4/29/20.
//  Copyright © 2020 Azumio Inc. All rights reserved.
//

import UIKit
import FoodLensSDK

class SceneDelegate: UIResponder, UIWindowSceneDelegate, FoodLensDelegate, SwitchboardViewControllerDelegate {

    var window: UIWindow?

    func scene(_ scene: UIScene, willConnectTo session: UISceneSession, options connectionOptions: UIScene.ConnectionOptions) {
        if let foodLens = FoodLens.lastAuthorizedInstance() {
            login(withFoodLens: foodLens)
        }
        else {
            FoodLens.authorizedInstance { (foodLens, error) in
                if let error = error {
                    let alert = UIAlertController(title: NSLocalizedString("Error", comment: "Error title"), message: NSLocalizedString(error.localizedDescription, comment: "Error message"), preferredStyle: .alert)
                    self.window?.rootViewController?.present(alert, animated: true, completion: {})
                }
                else if let foodLens = foodLens {
                    self.login(withFoodLens: foodLens)
                }
            }
        }
    }

    // MARK: SwitchboardViewController Delegate

    func switchboardViewControllerDidSelect(_ viewController: SwitchboardViewController, option: SwitchboardViewController.Options) {
        let viewController = foodLens.instantiateFoodSearchViewController()
        let navigationController = UINavigationController.foodLensNavigationController(withRootViewController: viewController)
        navigationController.modalPresentationStyle = .fullScreen
        window?.rootViewController?.present(navigationController, animated: true) {}
    }

    // MARK: FoodLens Delegate

    func foodLensPut(_ foodLens: FoodLens, foodCheckIn: FoodLensFoodCheckIn, from viewController: UIViewController?) {
        let resultViewController = storyboard.instantiateViewController(withIdentifier: "Result") as! ResultViewController
        resultViewController.object = foodCheckIn
        DispatchQueue.main.asyncAfter(deadline: .now() + 0.5) {
            self.window?.rootViewController?.present(resultViewController, animated: true, completion: {})
        }
    }
    
    func foodLensDelete(_ foodLens: FoodLens, foodCheckIn: FoodLensFoodCheckIn, from viewController: UIViewController?) {
    }

    // MARK: Private

    private var foodLens: FoodLens!
    private lazy var storyboard = UIStoryboard(name: "Examples", bundle: nil)

    private func login(withFoodLens foodLens: FoodLens) {
        self.foodLens = foodLens
        foodLens.delegate = self
        (window?.rootViewController as? SwitchboardViewController)?.delegate = self
        (window?.rootViewController as? SwitchboardViewController)?.options = [.search]
    }
}
