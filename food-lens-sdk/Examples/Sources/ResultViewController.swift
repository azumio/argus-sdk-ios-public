//
//  ResultViewController.swift
//  AICamera
//
//  Created by Vyacheslav Kuleshov on 5/20/20.
//  Copyright © 2020 Azumio Inc. All rights reserved.
//

import UIKit
import FoodLensSDK

class ResultViewController: UIViewController {

    @IBOutlet private weak var textView: UITextView!
    
    var object: FoodLensObject! {
        didSet {
            guard isViewLoaded else {return}
            updateTextView()
        }
    }

    override func viewDidLoad() {
        super.viewDidLoad()
        updateTextView()
    }

    private func updateTextView() {
        do {
            let data = try JSONSerialization.data(withJSONObject: object.dictionaryRepresentation, options: [.prettyPrinted])
            textView.text = String(data: data, encoding: .utf8)
        }
        catch (let error) {
            textView.text = "ERROR: \(error.localizedDescription)"
        }
    }
}
