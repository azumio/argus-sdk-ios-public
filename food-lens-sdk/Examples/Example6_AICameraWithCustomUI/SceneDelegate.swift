//
//  SceneDelegate.swift
//  FoodLensSDKExample
//
//  Created by Vyacheslav Kuleshov on 4/29/20.
//  Copyright © 2020 Azumio Inc. All rights reserved.
//

import UIKit
import FoodLensSDK

class SceneDelegate: UIResponder, UIWindowSceneDelegate, FoodLensDelegate {

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

    // MARK: FoodLens Delegate

    func foodLensPut(_ foodLens: FoodLens, foodCheckIn: FoodLensFoodCheckIn, from viewController: UIViewController?) {
        let resultViewController = storyboard.instantiateViewController(withIdentifier: "Result") as! ResultViewController
        resultViewController.object = foodCheckIn
        window?.rootViewController?.present(resultViewController, animated: true, completion: {})
    }
    
    func foodLensDelete(_ foodLens: FoodLens, foodCheckIn: FoodLensFoodCheckIn, from viewController: UIViewController?) {
    }

    func foodLens(_ foodLens: FoodLens, cameraViewControllerContentInset cameraViewController: UIViewController) -> UIEdgeInsets {
        return UIEdgeInsets(top: 0, left: 0, bottom: 70, right: 0)
    }

    // MARK: Private

    private var foodLens: FoodLens!
    private lazy var storyboard = UIStoryboard(name: "Examples", bundle: nil)

    private func login(withFoodLens foodLens: FoodLens) {
        self.foodLens = foodLens
        foodLens.delegate = self
        
        let rootViewController = storyboard.instantiateViewController(withIdentifier: "CameraExteriorViewController") as! CameraExteriorViewController
        rootViewController.cameraViewController = foodLens.instantiateCameraViewController(options: nil)
        window?.rootViewController = rootViewController
        rootViewController.cameraViewController?.navigationItem.hidesBackButton = true
    }
}
