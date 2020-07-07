//
//  ChildrenTouchableView.swift
//  FoodLensSDKExample
//
//  Created by Vyacheslav Kuleshov on 4/30/20.
//  Copyright © 2020 Azumio. All rights reserved.
//

import UIKit

open class ChildrenTouchableView: UIView {

    override open func point(inside point: CGPoint, with event: UIEvent?) -> Bool {
        if clipsToBounds && !bounds.contains(point) {
            return false
        }
        for subview in subviews {
            if subview.isUserInteractionEnabled && subview.point(inside: convert(point, to: subview), with: event) {
                return true
            }
        }
        return false
    }

    override open func hitTest(_ point: CGPoint, with event: UIEvent?) -> UIView? {
        let result = super.hitTest(point, with: event)
        if let result = result {
            return result === self ? nil : (result.isUserInteractionEnabled ? result : Optional<UIView>.none)
        }
        return result
    }
}
