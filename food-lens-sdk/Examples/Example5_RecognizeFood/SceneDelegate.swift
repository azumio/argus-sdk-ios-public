//
//  SceneDelegate.swift
//  FoodLensSDKExample
//
//  Created by Vyacheslav Kuleshov on 4/29/20.
//  Copyright © 2020 Azumio Inc. All rights reserved.
//

import UIKit
import Photos
import MobileCoreServices
import FoodLensSDK

class Segments: NSObject, FoodLensObject {
    required init?(coder: NSCoder) {preconditionFailure()}
    func encode(with coder: NSCoder) {preconditionFailure()}
    
    let segments: [FoodLensFoodSegment]
    init(segments: [FoodLensFoodSegment]) {
        self.segments = segments
    }
    var dictionaryRepresentation: [AnyHashable : Any] {
        return ["segments": segments.map {$0.dictionaryRepresentation}]
    }
}

class SceneDelegate: UIResponder, UIWindowSceneDelegate, SwitchboardViewControllerDelegate, UIImagePickerControllerDelegate, UINavigationControllerDelegate {

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
        let pick = {
            let imagePickerController = UIImagePickerController()
            imagePickerController.sourceType = .photoLibrary
            imagePickerController.delegate = self
            imagePickerController.mediaTypes = [kUTTypeImage as String]
            self.window?.rootViewController?.present(imagePickerController, animated: true, completion: {})
        }

        switch PHPhotoLibrary.authorizationStatus() {
            case .notDetermined:
                PHPhotoLibrary.requestAuthorization { (status) in
                    if status == .authorized {
                        DispatchQueue.main.async {
                            pick()
                        }
                    }
                }
            case .authorized:
                pick()
            default:
                let alertController = UIAlertController(title: NSLocalizedString("Permission denied", comment: "Alert title"), message: "", preferredStyle: .alert)
                alertController.addAction(UIAlertAction(title: NSLocalizedString("OK", comment: "Alert action"), style: .default) {_ in})
                alertController.addAction(UIAlertAction(title: NSLocalizedString("Open Settings", comment: "Alert action"), style: .default) {_ in
                    UIApplication.shared.open(URL(string: UIApplication.openSettingsURLString)!, options:[:]) {_ in}
                })
                window?.rootViewController?.present(alertController, animated: true, completion: {})
        }
    }

    // MARK: ImagePicker Delegate

    func imagePickerController(_ picker: UIImagePickerController, didFinishPickingMediaWithInfo info: [UIImagePickerController.InfoKey : Any]) {
        picker.presentingViewController?.dismiss(animated: true) {
            if let asset = info[UIImagePickerController.InfoKey.phAsset] as? PHAsset {
                PHImageManager.default().requestImageData(for: asset, options:PHImageRequestOptions()) { (imageData, dataUTI, orientation, info) in
                    if let imageData = imageData, let image = UIImage(data: imageData) {
                        self.foodLens.recognizeFood(image) { (segments, error) in
                            if let error = error {
                                let alert = UIAlertController(title: NSLocalizedString("Error", comment: "Error title"), message: NSLocalizedString(error.localizedDescription, comment: "Error message"), preferredStyle: .alert)
                                alert.addAction(UIAlertAction(title: NSLocalizedString("OK", comment: "Error title"), style: .default, handler: { (_) in}))
                                self.window?.rootViewController?.present(alert, animated: true, completion: {})
                            }
                            else if let segments = segments {
                                let resultViewController = self.storyboard.instantiateViewController(withIdentifier: "Result") as! ResultViewController
                                resultViewController.object = Segments(segments: segments)
                                self.window?.rootViewController?.present(resultViewController, animated: true, completion: {})
                            }
                        }
                    }
                }
            }
        }
    }
    
    func imagePickerControllerDidCancel(_ picker: UIImagePickerController) {
        picker.presentingViewController?.dismiss(animated: true, completion: {})
    }

    // MARK: Private

    private var foodLens: FoodLens!
    private lazy var storyboard = UIStoryboard(name: "Examples", bundle: nil)

    private func login(withFoodLens foodLens: FoodLens) {
        self.foodLens = foodLens
        (window?.rootViewController as? SwitchboardViewController)?.delegate = self
        (window?.rootViewController as? SwitchboardViewController)?.options = [.recognize]
    }
}
