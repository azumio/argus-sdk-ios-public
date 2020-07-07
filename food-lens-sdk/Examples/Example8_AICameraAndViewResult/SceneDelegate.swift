//
//  SceneDelegate.swift
//  FoodLensSDKExample
//
//  Created by Vyacheslav Kuleshov on 6/24/20.
//  Copyright © 2020 Azumio Inc. All rights reserved.
//

import UIKit
import MobileCoreServices
import FoodLensSDK

class SceneDelegate: UIResponder, UIWindowSceneDelegate, SwitchboardViewControllerDelegate, FoodLensDelegate {

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
        switch option {
            case .camera:
                let viewController = foodLens.instantiateCameraViewController(options: nil)
                viewController.modalPresentationStyle = .fullScreen
                window?.rootViewController?.present(viewController, animated: true) {}
            case .viewResult:
                guard let lastFoodCheckIn = lastFoodCheckIn else {break}
                let viewController = foodLens.instantiateResultViewController(forCheckIn: lastFoodCheckIn)
                let navigationController = UINavigationController.foodLensNavigationController(withRootViewController: viewController)
                navigationController.modalPresentationStyle = .fullScreen
                window?.rootViewController?.present(navigationController, animated: true) {}
            default:
                break
        }
    }

    // MARK: FoodLens Delegate

    func foodLensPut(_ foodLens: FoodLens, foodCheckIn: FoodLensFoodCheckIn, from viewController: UIViewController?) {
        lastFoodCheckIn = foodCheckIn
    }
    
    func foodLensDelete(_ foodLens: FoodLens, foodCheckIn: FoodLensFoodCheckIn, from viewController: UIViewController?) {
        lastFoodCheckIn = nil
    }

    // MARK: Private

    private var foodLens: FoodLens!
    private var lastFoodCheckIn: FoodLensFoodCheckIn? {
        didSet {
            if lastFoodCheckIn != nil {
                (window?.rootViewController as? SwitchboardViewController)?.options = [.camera, .viewResult]
            }
            else {
                (window?.rootViewController as? SwitchboardViewController)?.options = [.camera]
            }
        }
    }
    private lazy var storyboard = UIStoryboard(name: "Examples", bundle: nil)

    private func login(withFoodLens foodLens: FoodLens) {
        self.foodLens = foodLens
        foodLens.delegate = self
        (window?.rootViewController as? SwitchboardViewController)?.delegate = self
        (window?.rootViewController as? SwitchboardViewController)?.options = [.camera]
    }
}
