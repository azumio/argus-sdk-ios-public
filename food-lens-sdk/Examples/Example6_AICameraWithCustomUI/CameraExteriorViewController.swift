//
//  CameraExteriorViewController.swift
//  FoodLensSDKExample
//
//  Created by Vyacheslav Kuleshov on 4/30/20.
//  Copyright © 2020 Azumio Inc. All rights reserved.
//

import UIKit

class CameraExteriorViewController: UIViewController {

    @IBOutlet weak var cameraContainerView: UIView!
    @IBOutlet weak var foodButton: UIButton!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        installCameraViewController()
        updateControls()
    }

    var cameraViewController: UIViewController? {
        didSet {
            guard isViewLoaded else {return}
            installCameraViewController()
        }
    }

    // MARK: Private
    
    private func updateControls() {
        guard isViewLoaded else {return}
    }
    
    private func installCameraViewController() {
        guard let cameraViewController = cameraViewController else {return}

        addChild(cameraViewController)
        cameraViewController.view.translatesAutoresizingMaskIntoConstraints = false
        cameraContainerView.addSubview(cameraViewController.view)
        cameraViewController.view.leftAnchor.constraint(equalTo: cameraContainerView.leftAnchor).isActive = true
        cameraViewController.view.rightAnchor.constraint(equalTo: cameraContainerView.rightAnchor).isActive = true
        cameraViewController.view.topAnchor.constraint(equalTo: cameraContainerView.topAnchor).isActive = true
        cameraViewController.view.bottomAnchor.constraint(equalTo: cameraContainerView.bottomAnchor).isActive = true
        cameraViewController.didMove(toParent: self)
    }
    
    @IBAction func foodTap(_ sender: UIButton) {
        foodButton.setTitle(sender.tag == 1 ? "🍒" : sender.title(for: .normal), for: .normal)
    }
}
