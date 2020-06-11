//
//  FoodLens.h
//  FoodLensSDK
//
//  Created on 10/16/19.
//  Copyright © 2019 Azumio Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@protocol FoodLensDataSource;
@protocol FoodLensMutableDataSource;
@protocol FoodLensFoodCheckIn;
@protocol FoodLensFoodSegment;
@class FoodLens;

/**
    Pass objects confroming to this protocol to @c FoodLens instance in order to handle results from UI
 */
@protocol FoodLensDelegate <NSObject>
/**
    This method is called when FoodLens UI is ready to put updated or new data
    
    Typical implementation should store foodCheckIn on its own or put inside of mutable data source like one returned from instantiateOnlineHistoryDataSource
    Implementors should take care of proper storing of checkins and returning them on data source behalf in chronological order, from most recent to the old ones

    @code
    func foodLensPut(_ foodLens: FoodLens, foodCheckIn: FoodLensFoodCheckIn, from viewController: UIViewController?) {
        (foodLens.dataSource as? FoodLensMutableDataSource)?.foodLensPut(foodLens, checkIn: foodCheckIn)
    }
    @endcode

    @param foodLens The foodLens instance
    @param foodCheckIn A new or updated data. It's implementor responsibility to store the data. Or if @c FoodLensMutableDataSource is used then put it with @c foodLensPut:checkIn:
    @param viewController A view controller which is ready to put data
 */
- (void)foodLensPut:(nonnull FoodLens *)foodLens foodCheckIn:(nonnull id<FoodLensFoodCheckIn>)foodCheckIn fromViewController:(nullable UIViewController *)viewController;
/**
    This method is called when FoodLens UI is ready to delete data
    
    Typical implementation should delete foodCheckIn on its own or delete from mutable data source like one returned from instantiateOnlineHistoryDataSource
    Implementors should take care of proper storing of checkins and returning them on data source behalf in chronological order, from most recent to the old ones

    @code
    func foodLensDelete(_ foodLens: FoodLens, foodCheckIn: FoodLensFoodCheckIn, from viewController: UIViewController?) {
        (foodLens.dataSource as? FoodLensMutableDataSource)?.foodLensDelete(foodLens, checkIn: foodCheckIn)
    }
    @endcode

    @param foodLens The foodLens instance
    @param foodCheckIn A data to be deleted. It's implementor responsibility to delete the data. Or if @c FoodLensMutableDataSource is used then put it with @c foodLensDelete:checkIn:
    @param viewController A view controller which is ready to delete data
 */
- (void)foodLensDelete:(nonnull FoodLens *)foodLens foodCheckIn:(nonnull id<FoodLensFoodCheckIn>)foodCheckIn fromViewController:(nullable UIViewController *)viewController;
@end

/**
    The entry point of FoodLens SDK.

    Initialize FoodLens for the first time using @c +authorizedInstanceWithAccessToken:.
    Next time access the last authorized instance with @c+lastAuthorizedInstance.
    @c +lastAuthorizedInstance cleans up the last stored instance.

    FoodLens interface is divided onto two parts. UI and low level UI less.
    
    @c -instatiateViewController methods are intended to instatiate the UI part.
    View Controllers take required data from dataSource and return new or updated data to delegate.
    It's a user responsibility to store the data back.

    FoodLens gives convenient way to manage and store data online.
    @c -instantiateOnlineHistoryDataSource instantiates data source which loads online data.
    It also conform to @c FoodLensMutableDataSource protocol. So it's possible to store the data returned to delegate online.

    @c -recognizeFood:withCompletion: is a UI less method for recognizing food on the given image.
 */
@interface FoodLens : NSObject

/**
    The data source.
    
    FoodLens calls to data source when UI needs data.
    The most common way is to use online data source provided by FoodLens. Just instantiate data source with @c instantiateOnlineHistoryDataSource and assign to @c dataSource
    In case of custom data source implementation, implementors should take care of proper data ordering and access by identifier.
    
    The example projects perfectly demonstrates both ways.
 */
@property (nullable, nonatomic, weak) id<FoodLensDataSource> dataSource;
/**
    The delegate.
    
    FoodLens calls to delegate when UI is ready to delete or put new or updated data.
    The most common way is to use online data source provided by FoodLens with @c instantiateOnlineHistoryDataSource method. Delegate implementation should just call its mutable methods.
    In case of custom data source implementation, implementors should take care of proper data ordering and access by identifier.
    
    The example projects perfectly demonstrates both ways.
 */
@property (nullable, nonatomic, weak) id<FoodLensDelegate> delegate;
/**
    The access token which the FoodLens instance was initialized with for the first time.
 */
@property (nullable, nonatomic, strong, readonly) NSString *accessToken;

/**
    Returns last authorized FoodLens instance.
    
    The instance is saved across launches. FoodLens should be initialized with @c authorizedInstanceWithAccessToken: for the first time.
    Call @c logout to remove the last instance and cleanup some internal stuff.
    Typical implementation can looks like this
    
    @code
    FoodLens *foodLens = [FoodLens lastAuthorizedInstance];
    if (!foodLens) {
        [self requestNewAccessToken:^(NSString *token) {
            foodLens = [FoodLens authorizedInstanceWithAccessToken: token];
            // Start working with FoodLens
            ...
        }];
    }
    else {
        // Start working with FoodLens
        ...
    }
    @endcode
    
    @return Returns last authorized FoodLens instance.
 */
+ (nullable instancetype)lastAuthorizedInstance;

/**
    Returns new authorized FoodLens instance.

    Call it for the first time when @c lastAuthorizedInstance returns nil. @c lastAuthorizedInstance should be called next time.
    @see lastAuthorizedInstance
    
    The accessToke should be received from the designated web services.
    Example projects demonstrate how it can be obtained.
        
    Shortly:
    @code
    var request = URLRequest(url: Self.endpoint.appendingPathComponent("api2/token"))
    request.setValue("application/json", forHTTPHeaderField: "Content-Type")
    request.httpMethod = "POST"
    let payload = TokenPayload(user_id: userId)
    
    let task = urlSession.uploadTask(with: request, from: try! JSONEncoder().encode(payload)) { (responseData, response, error) in
        if let data = responseData {
            do {
                let response = try JSONDecoder().decode(TokenResponse.self, from: data)
                let foodLens = FoodLens.authorizedInstance(withAccessToken: response.access_token)

                // Start working with FoodLens
                ...
            } catch (let e) {
                ...
            }
        }
        ...
    }
    task.resume()
    @endcode
    
    @param token An accessToken Should be received from the designated web services

    @return Returns new authorized FoodLens instance.
 */
+ (nonnull instancetype)authorizedInstanceWithAccessToken:(nonnull NSString *)token;

/**
    Returns new authorized FoodLens instance.

    Call it for the first time when @c lastAuthorizedInstance returns nil. @c lastAuthorizedInstance should be called next time.
    @see lastAuthorizedInstance
 */
+ (void)logout;

/**
    Instantiates FoodLens online data source.
    
    The method just creates but doesn't assign the result anywhere.

    From the other hand the result conforms to @c FoodLensMutableDataSource thus it can be used to put or delete updated data from delegate implementations.
    After instatation, the result should be assigned to the @c dataSource property
    
    Using of online data source and thus this method is optional. Custom implementations of @c FoodLensDataSource can be asssigned to @c dataSource

    @return Returns new data source instance.
 */
- (nonnull id<FoodLensMutableDataSource>)instantiateOnlineHistoryDataSource;

/**
    Instantiates AI camera view controller.
    
    The resulting view controller helps to make food photo and display result to user. User then can choose what food item it can actually log, what serving size, etc.
    The controller can call data source to get existing data and calls delegate when it's sbout to finish food selection.

    Delegate is responsible for storing new data.

    @return Returns new view controller instance.
 */
- (nonnull UIViewController *)instantiateCameraViewController;

/**
    Instantiates history view controller.
    
    The resulting view controller display history of the taken food photos and user's choices on serving sizes, etc..
    The controller offers to user exising data modification as well as removing of certain items.
    Data source is called to get existing data and delegate is called when it's sbout to finish modifications.

    Delegate is responsible for storing of updated and deleted data.

    @return Returns new view controller instance.
 */
- (nonnull UIViewController *)instantiateHistoryViewController;

/**
    Instantiates search food view controller.
    
    The resulting view controller allows user to search for food and decide what it want to log, what serving size, etc.
    The controller calls delegate when it's sbout to finish food selection.
    For better UX it's recommended to put the result to the specific navigation controller instantiated with @c +[UINavigationController foodNavigationControllerWithRootViewController: ].

    Delegate is responsible for storing new data.
 */
- (nonnull UIViewController *)instantiateFoodSearchViewController;

/**
    UI less method for the food recognition.
    
    The receiver tries to find food on the given image aynchronously using authorized web services and returns result to user.
    Each segment in the completion represents separate food on the given image.

    @see FoodLensFoodSegment

    @param image An image to be used for recognition
    @param completion A completion. Is called with either @c error or @c segments initialized. Segments represents all food items recognized on the photo. Every segment contains a list of food variants which are offered to user for confirmation.
 */
- (void)recognizeFood:(nonnull UIImage *)image withCompletion:(nonnull void(^)(NSArray<id<FoodLensFoodSegment>> *_Nullable segments, NSError *_Nullable error))completion;

@end
