//
//  HistoryDataSource.swift
//  FoodLensExample
//
//  Created by Vyacheslav Kuleshov on 12/5/19.
//  Copyright © 2019 Azumio Inc. All rights reserved.
//

import UIKit
import FoodLensSDK

class HistoryDataSource: NSObject, FoodLensMutableDataSource {

    override private init() {
        super.init()
    }

    static var `default`: HistoryDataSource {
        return HistoryDataSource()
    }

    private lazy var history: History = {
        do {
            let data = try Data(contentsOf: URL(fileURLWithPath: historyStoragePath))
            let unarchiver = try NSKeyedUnarchiver(forReadingFrom: data)
            return unarchiver.decodeDecodable(History.self, forKey: NSKeyedArchiveRootObjectKey)!
        } catch {
            return History()
        }
    }()

    private func saveHistory() {
        do {
            let archiver = NSKeyedArchiver(requiringSecureCoding: false)
            archiver.outputFormat = .binary
            try archiver.encodeEncodable(history, forKey: NSKeyedArchiveRootObjectKey)
            archiver.finishEncoding()

            try archiver.encodedData.write(to: URL(fileURLWithPath: historyStoragePath))
        } catch {
            print("Couldn't write history storage: \(error)")
        }
    }

    private lazy var historyStoragePath: String = {
        let documentsFolder = NSSearchPathForDirectoriesInDomains(.documentDirectory, [.userDomainMask], true).first! as NSString
        return documentsFolder.appendingPathComponent("history.storage") as String
    }()

    func purge() {
        try? FileManager.default.removeItem(atPath: historyStoragePath)
    }

    // MARK: FoodLens DataSource

    func foodLensFoodCheckIns(_ foodLens: FoodLens) -> [FoodLensFoodCheckIn] {
        return history.orderedCheckInsOfType()
    }
    
    func foodLensFoodCheckIn(_ foodLens: FoodLens, forDayOnDate date: Date) -> [FoodLensFoodCheckIn] {
        let dayNumber = date.dayNumberSince1970
        let foodCheckIns: [FoodLensFoodCheckIn] = history.orderedCheckInsOfType()
        return foodCheckIns.filter {$0.date.dayNumberSince1970(inTimeZone: $0.timeZone) == dayNumber}
    }
    
    func foodLensFoodCheck(in foodLens: FoodLens, forIdentifier identifier: String) -> FoodLensFoodCheckIn? {
        history.checkIn(withIdentifier: identifier) as? FoodLensFoodCheckIn
    }

    func foodLensPut(_ foodLens: FoodLens, checkIn: FoodLensCheckIn) {
        history.put(checkIn: checkIn)
    }
    
    func foodLensDelete(_ foodLens: FoodLens, checkIn: FoodLensCheckIn) {
        history.delete(withIdentifier: checkIn.identifier)
    }

    func foodLensSave(_ foodLens: FoodLens) {
        saveHistory()
    }
}

private extension Date {
    var dayNumberSince1970: Int {
        return dayNumberSince1970(inTimeZone: nil)
    }

    func dayNumberSince1970(inTimeZone timeZone: TimeZone?) -> Int {
        var calendar = NSCalendar.current
        if let timeZone = timeZone {
            calendar.timeZone = timeZone
        }
        let difference = calendar.dateComponents([.day], from: Date(timeIntervalSince1970: 0), to: self)
        return difference.day!
    }
}
