//
//  SwitchboardViewController.swift
//  FoodLensSDKExample
//
//  Created by Vyacheslav Kuleshov on 4/30/20.
//  Copyright © 2020 Azumio Inc. All rights reserved.
//

import UIKit
import FoodLensSDK

protocol SwitchboardViewControllerDelegate: class {
    func switchboardViewControllerDidSelect(_ viewController: SwitchboardViewController, option: SwitchboardViewController.Options)
}

extension SwitchboardViewControllerDelegate {
    func switchboardViewControllerDidSelect(_ viewController: SwitchboardViewController, option: SwitchboardViewController.Options) {}
}

class SwitchboardViewController: UIViewController {

    weak var delegate: SwitchboardViewControllerDelegate?

    struct Options: OptionSet, Hashable {
        public let rawValue: Int

        static let camera = Options(rawValue: 1 << 0)
        static let history = Options(rawValue: 1 << 1)
        static let search = Options(rawValue: 1 << 2)
        static let recognize = Options(rawValue: 1 << 3)

        public init(rawValue: Int) {self.rawValue = rawValue}
    }

    var options: Options = [] {
        didSet {
            updateControls()
        }
    }
    
    private lazy var buttonsByOption: [Options: UIButton] = Dictionary(uniqueKeysWithValues: stackView.arrangedSubviews.map {(Options(rawValue: $0.tag), $0 as! UIButton)})

    @IBOutlet private weak var activityIndicator: UIActivityIndicatorView!
    @IBOutlet private weak var stackView: UIStackView!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        updateControls()
    }

    @IBAction func optionTap(_ sender: UIView) {
        delegate?.switchboardViewControllerDidSelect(self, option: Options(rawValue: sender.tag))
    }

    // MARK: Private
    
    private func updateControls() {
        guard isViewLoaded else {return}
        for (option, button) in buttonsByOption {
            button.isHidden = !options.contains(option)
        }
        activityIndicator.isHidden = !options.isEmpty
    }
}
