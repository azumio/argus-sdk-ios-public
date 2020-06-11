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

    func sceneWillResignActive(_ scene: UIScene) {
        foodLensDataSource?.foodLensSave(foodLens)
    }

    // MARK: SwitchboardViewController Delegate

    func switchboardViewControllerDidSelect(_ viewController: SwitchboardViewController, option: SwitchboardViewController.Options) {
        switch option {
            case .camera:
                let viewController = foodLens.instantiateCameraViewController()
                viewController.modalPresentationStyle = .fullScreen
                window?.rootViewController?.present(viewController, animated: true) {}
            case .search:
                let viewController = foodLens.instantiateFoodSearchViewController()
                let navigationController = UINavigationController.foodNavigationController(withRootViewController: viewController)
                navigationController.modalPresentationStyle = .fullScreen
                window?.rootViewController?.present(navigationController, animated: true) {}
            case .history:
                let viewController = foodLens.instantiateHistoryViewController()
                viewController.modalPresentationStyle = .fullScreen
                window?.rootViewController?.present(viewController, animated: true) {}
            default:
                break
        }
    }

    // MARK: FoodLens Delegate

    func foodLensPut(_ foodLens: FoodLens, foodCheckIn: FoodLensFoodCheckIn, from viewController: UIViewController?) {
        foodLensDataSource.foodLensPut(foodLens, checkIn: foodCheckIn)
    }
    
    func foodLensDelete(_ foodLens: FoodLens, foodCheckIn: FoodLensFoodCheckIn, from viewController: UIViewController?) {
        foodLensDataSource.foodLensDelete(foodLens, checkIn: foodCheckIn)
    }

    // MARK: Private

    private var foodLens: FoodLens!
    private var foodLensDataSource: FoodLensMutableDataSource!
    private lazy var storyboard = UIStoryboard(name: "Examples", bundle: nil)

    private func login(withFoodLens foodLens: FoodLens) {
        self.foodLens = foodLens
        foodLensDataSource = foodLens.instantiateOnlineHistoryDataSource()
        foodLens.delegate = self
        foodLens.dataSource = self.foodLensDataSource
        (window?.rootViewController as? SwitchboardViewController)?.delegate = self
        (window?.rootViewController as? SwitchboardViewController)?.options = [.camera, .search, .history]
    }
}
