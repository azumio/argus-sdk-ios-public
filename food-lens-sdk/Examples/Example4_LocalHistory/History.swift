//
//  History.swift
//  FoodLensExample
//
//  Created by Vyacheslav Kuleshov on 10/17/19.
//  Copyright © 2019 Azumio Inc. All rights reserved.
//

import Foundation
import FoodLensSDK

class History: NSObject, Codable {

    var orderedCheckIns: [FoodLensCheckIn] = []

    override init() {
        super.init()
    }

    func checkIn(withIdentifier identifier: String) -> FoodLensCheckIn? {
        return orderedCheckIns.first {$0.identifier == identifier}
    }

    func orderedCheckInsOfType<T>() -> [T] {
        return orderedCheckIns.lazy.filter {$0.conforms(to: FoodLensCheckIn.self)}.map {$0 as! T}
    }

    func orderedCheckInsOfType<T: FoodLensFoodCheckIn>() -> [T] {
        return orderedCheckIns.lazy.filter {$0.conforms(to: FoodLensFoodCheckIn.self)}.map {$0 as! T}
    }

    func put(checkIn: FoodLensCheckIn) {
        if let existingCheckInIndex = orderedCheckIns.firstIndex(where: {$0.identifier == checkIn.identifier}) {
            orderedCheckIns[existingCheckInIndex] = checkIn
        }
        else if let index = orderedCheckIns.firstIndex(where: {$0.date < checkIn.date}) {
            orderedCheckIns.insert(checkIn, at: index)
        }
        else {
            orderedCheckIns.append(checkIn)
        }
    }

    func delete(withIdentifier identifier: String) {
        if let existingCheckInIndex = orderedCheckIns.firstIndex(where: {$0.identifier == identifier}) {
            orderedCheckIns.remove(at: existingCheckInIndex)
        }
    }

    // MARK: Coding

    enum CodingKeys: String, CodingKey {
        case orderedCheckIns
    }

    func encode(to encoder: Encoder) throws {
        var container = encoder.container(keyedBy: CodingKeys.self)
        let checkInDatas = try orderedCheckIns.map {try NSKeyedArchiver.archivedData(withRootObject: $0, requiringSecureCoding: false)}
        try container.encode(checkInDatas, forKey: .orderedCheckIns)
    }

    required init(from decoder: Decoder) throws {
        let container = try decoder.container(keyedBy: CodingKeys.self)
        let checkInDatas = try container.decode([Data].self, forKey: .orderedCheckIns)
        orderedCheckIns = try checkInDatas.map {try NSKeyedUnarchiver.unarchiveTopLevelObjectWithData($0) as! FoodLensCheckIn}
    }
}
