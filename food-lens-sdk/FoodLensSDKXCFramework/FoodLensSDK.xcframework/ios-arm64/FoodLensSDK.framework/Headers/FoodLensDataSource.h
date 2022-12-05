//
//  FoodLensDataSource.h
//  FoodLensSDK
//
//  Created by Vyacheslav Kuleshov on 12/5/19.
//  Copyright © 2019 Azumio Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class FoodLens;
@protocol FoodLensCheckIn;
@protocol FoodLensFoodCheckIn;

/**
    Protocol adopted by objects in order to provide data for FoodLens UI
    
    Object adopting this protocol should be assigned to the @c FoodLens @c dataSource property to start providing the data.
    For the online timeline provided by FoodLens, there is already ready to use data source implementation which could be created with @c -[FoodLens instantiateOnlineHistoryDataSource] method.
 */
@protocol FoodLensDataSource <NSObject>
/**
    Tells the data source to return all available data
    
    The data must be returned ordered from the most recent to the oldest. Otherwise it may result in unexpected behavior.
    It's up to implementors store data in optimized way.
    
    @param foodLens The FoodLens object requesting the data

    @return Should return ordered list of @c FoodLensFoodCheckIn conforming objects. The most recent must be first.
 */
- (nonnull NSArray<id<FoodLensFoodCheckIn>> *)foodLensFoodCheckIns:(nonnull FoodLens *)foodLens;

/**
    Tells the data source to return data available on the certain date.
    
    The data must be returned ordered from the most recent to the oldest. Otherwise it may result in unexpected behavior.
    It's up to implementors store data in optimized way.
    
    The day means a day which the given date belongs to in current timezone.
    
    @note CheckIn timezone may differ depending on the timezone where it was created ot updated. @ -[FoodLensCheckIn timeZone] has to be considered when determining the day which checkin belongs to.
    
    @param foodLens The FoodLens object requesting the data
    @param date A date which the requesting day has to belong to in current timezone.

    @return Should return ordered list of @c FoodLensFoodCheckIn conforming objects. The most recent must be first.
 */
- (nonnull NSArray<id<FoodLensFoodCheckIn>> *)foodLensFoodCheckIns:(nonnull FoodLens *)foodLens forDayOnDate:(nonnull NSDate *)date NS_SWIFT_NAME(foodLensFoodCheckIn(_:forDayOnDate:));

/**
    Tells the data source to return checkin by unique identifier.
    
    Each checkin should be uniquely identified with @ -[FoodLensCheckIn identifier]  identifier.

    @param foodLens The FoodLens object requesting the data
    @param identifier An identifier of checkin

    @return Should return ordered list of @c FoodLensFoodCheckIn conforming objects. The most recent must be first.
 */
- (nullable id<FoodLensFoodCheckIn>)foodLensFoodCheckIn:(nonnull FoodLens *)foodLens forIdentifier:(nonnull NSString *)identifier;
@end

/**
    Protocol declaring mutable dataSource methods for storing or deleting updated or new data.
    
    Actually no one of FoodLens UI requires mutable dataSource. But online dataSource instances returned from @c -[FoodLens instantiateOnlineHistoryDataSource] conforms to the mutable protocol. So FoodLens delegate implementation can just put or delete data right to the online data source.
    
    @code
    func foodLensPut(_ foodLens: FoodLens, foodCheckIn: FoodLensFoodCheckIn, from viewController: UIViewController?) {
        (foodLens.dataSource as? FoodLensMutableDataSource)?.foodLensPut(foodLens, checkIn: foodCheckIn)
    }
    
    func foodLensDelete(_ foodLens: FoodLens, foodCheckIn: FoodLensFoodCheckIn, from viewController: UIViewController?) {
        (foodLens.dataSource as? FoodLensMutableDataSource)?.foodLensDelete(foodLens, checkIn: foodCheckIn)
    }
    @endcode
    
    Object adopting this protocol should be assigned to the @c FoodLens @c dataSource property to start providing the data.
    For the online timeline provided by FoodLens, there is already ready to use data source implementation which could be created with @c -[FoodLens instantiateOnlineHistoryDataSource] method.
    
    @c FoodLensCheckIn declares conformance to @c NSCoding protocol. So custom implementation may use standard coders for encoding and decoding all the data.
 */
@protocol FoodLensMutableDataSource <FoodLensDataSource>
/**
    Tells the data source to store new or updated checkin.
    
    The receiver is responsible for storing checkins and returning them when they are requested with @c FoodLensDataSource methods

    @param foodLens The FoodLens object requesting data update
    @param checkIn A checkin to be stored
    
 */
- (void)foodLensPut:(nonnull FoodLens *)foodLens checkIn:(nonnull id<FoodLensCheckIn>)checkIn;

/**
    Tells the data source to delete the given checkin.
    
    The receiver is responsible for deleting checkin so it was not returned on next call of @c FoodLensDataSource methods

    @param foodLens The FoodLens object requesting data update
    @param checkIn A checkin to be deleted
    
 */
- (void)foodLensDelete:(nonnull FoodLens *)foodLens checkIn:(nonnull id<FoodLensCheckIn>)checkIn;

/**
    Tells the data source to save all data so that it was preserved between app relaunches.
    
    FoodLensCheckIn conforms to NSCoding protocol so the simplest implementation would be just encode and store all data.
    @note The receiver or owner is responsible for cleaning up it's data storage after +[FoodLens logout]
    
    The online datasource provided by FoodLens caches all data when this method is called.
    It's typical usage may looks like
    
    @code
    func sceneWillResignActive(_ scene: UIScene) {
        (foodLens.dataSource as? FoodLensMutableDataSource)?.foodLensSave(foodLens)
    }
    @endcode

    @param foodLens The FoodLens object requesting data saving
 */
- (void)foodLensSave:(nonnull FoodLens *)foodLens;
@end

