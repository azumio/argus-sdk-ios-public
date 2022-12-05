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
@protocol FoodLensFoodLog;
@protocol FoodLensFoodCategory;
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

@optional

/**
    This method is called by the CameraViewController to know how much to inset its controls relative to default
    
    Implementation can return non zero result in order to add some insets for controls. The freed space can be used for custom controls layouting.

    @param foodLens The foodLens instance
    @param cameraViewController A camera view controller which asks for insets
 */
- (UIEdgeInsets)foodLens:(nonnull FoodLens *)foodLens cameraViewControllerContentInset:(nonnull UIViewController *)cameraViewController;

/**
    This method is called by the FoodAR CameraViewController when new food instance is found in the focus

    Implementation is responsible for keeping it's own list of categories in sync with what is found by the camera view controller.
    View controller preserves all found instances unless @c [FoodLens foodARCameraViewControllerRestart] is called

    @param foodLens The foodLens instance
    @param cameraViewController A foodAR camera view controller which did find food instance
    @param category A category respresenting the found food instance
 */
- (void)foodLens:(nonnull FoodLens *)foodLens foodARCameraViewControllerDidFindFood:(nonnull UIViewController *)cameraViewController category:(nonnull id<FoodLensFoodCategory>)category;

/**
    This method is called by the FoodAR CameraViewController when previously found food instance was scanned again

    The view controller consults the previously found categories to decide if new one exists already
    Implementation is responsible for keeping it's own list of categories in sync with what is found or updated by the camera view controller.
    View controller preserves all found instances unless @c [FoodLens foodARCameraViewControllerRestart] is called

    @param foodLens The foodLens instance
    @param cameraViewController A foodAR camera view controller which did find food instance
    @param category A category respresenting the found food instance
 */
- (void)foodLens:(nonnull FoodLens *)foodLens foodARCameraViewControllerDidUpdateFood:(nonnull UIViewController *)cameraViewController category:(nonnull id<FoodLensFoodCategory>)category;

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

    It depends how the instance was first obtained to get it asynchronously or synchrnously on next launches. The most generic way is to try to get it synchronously with @c lastAuthorizedInstance.
    And then try the asynchronous way.

    More info can be found in descriptions for each specific @c authorized... method.

    The instance is saved across launches. FoodLens should be initialized with @c authorizedInstanceWithAccessToken: for the first time.
    Call @c logout to remove the last instance and cleanup some internal stuff.
    Typical implementation can looks like this
    
    @code
    FoodLens *foodLens = [FoodLens lastAuthorizedInstance];
    if (!foodLens) {
        [FoodLens lastAuthorizedInstanceWithCompletion:^(FoodLens *foodLens) {
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
    Returns last authorized FoodLens instance asynchronously.

    It depends how the instance was first obtained to get it asynchronously or synchrnously on next launches. The most generic way is to try to get it synchronously with @c lastAuthorizedInstance.
    And then try the asynchronous way.

    More info can be found in descriptions for each specific @c authorized... method.

    The instance is saved across launches. FoodLens should be initialized with @c authorized... methods for the first time.
    Call @c logout to remove the last instance and cleanup some internal stuff.
    Typical implementation can looks like this

    @code
    FoodLens *foodLens = [FoodLens lastAuthorizedInstance];
    if (!foodLens) {
        [FoodLens lastAuthorizedInstanceWithCompletion:^(FoodLens *foodLens) {
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
        }];
    }
    else {
        // Start working with FoodLens
        ...
    }
    @endcode

    @param completion A completion. There is one argument. May be nil if no instance were accessed previosly.
 */
+ (void)lastAuthorizedInstanceWithCompletion:(nonnull void(^)(FoodLens *_Nullable foodLens))completion;

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
                FoodLens.authorizedInstance(withAccessToken: response.access_token) { (foodLens, error) in
                    if let foodLens = foodLens {
                        // Start working with FoodLens
                    }
                    else ...
                }
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
    @param completion A completion. Is called with either authorized FoodLens instance or error.
 */
+ (void)authorizedInstanceWithAccessToken:(nonnull NSString *)token completion:(nonnull void(^)(FoodLens *_Nullable foodLens, NSError *_Nullable error))completion;

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

    @param options Currently should be nil or empty

    @return Returns new view controller instance.
 */
- (nonnull UIViewController *)instantiateCameraViewControllerWithOptions:(nullable NSDictionary *)options;


/**
    Instantiates result view controller.
    
    Allows to bypass camera screen and displays recognition result right away for the given image. User then can choose what food item it can actually log, what serving size, etc.
    The controller can call data source to get existing data and calls delegate when it's sbout to finish food selection.

    Delegate is responsible for storing new data.

    @param image An image for which to show the result
    @param options Currently should be nil or empty

    @return Returns new view controller instance.
 */
- (nonnull UIViewController *)instantiateResultViewControllerForImage:(nonnull UIImage *)image withOptions:(nullable NSDictionary *)options NS_SWIFT_NAME(instantiateResultViewController(forImage:withOptions:));

/**
    Instantiates result view controller.
    
    Opens result view controller for previously confirmed food checkin. The result is opened in view only mode. As an options user then can either edit or delete it.
    The delegate is notified about updated data and is responsible for storing it.

    @param checkIn A checkin for which to show the result

    @return Returns new view controller instance.
 */
- (nonnull UIViewController *)instantiateResultViewControllerForCheckIn:(nonnull id<FoodLensFoodCheckIn>)checkIn NS_SWIFT_NAME(instantiateResultViewController(forCheckIn:));

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
    Instantiates edit calories food view controller.

    The resulting view controller allows user to change serving kind and size for the given food log.
    For better UX it's recommended to put the result to the specific navigation controller instantiated with @c +[UINavigationController foodNavigationControllerWithRootViewController: ].

    @param foodLog A food log
    @param onDidEdit A completion. The view controller calls it when editing is done and passes changed log inside.
 */
- (nonnull UIViewController *)instantiateCalorieEditViewControllerForFoodLog:(nonnull id<FoodLensFoodLog>)foodLog onDidEdit:(nonnull void(^)(id<FoodLensFoodLog> _Nonnull))onDidEdit;


/**
    UI less method for the food recognition.
    
    The receiver tries to find food on the given image aynchronously using authorized web services and returns result to user.
    Each segment in the completion represents separate food on the given image.

    @see FoodLensFoodSegment

    @param image An image to be used for recognition
    @param completion A completion. Is called with either @c error or @c segments initialized. Segments represents all food items recognized on the photo. Every segment contains a list of food variants which are offered to user for confirmation.
 */
- (void)recognizeFood:(nonnull UIImage *)image withCompletion:(nonnull void(^)(NSArray<id<FoodLensFoodSegment>> *_Nullable segments, NSError *_Nullable error))completion;

/**
    A trap for the internally handled URLs.

    The host app should at least has its bundle identifier declared as a handled URL scheme in info plist's key @c CFBundleURLTypes
    and call this method from the AppDelegate method
    @code
    func application(_ app: UIApplication, open url: URL, options: [UIApplication.OpenURLOptionsKey : Any] = [:]) -> Bool {
        return foodLens?.open(url, options: options) ?? false
    }
    @endcode

    @param url An URL requsted to be opened
    @param options An options as it passed to the the AppDelegate method

    @return Returns true if the URL has been handled internally.
 */
- (BOOL)openURL:(nonnull NSURL *)url options:(nonnull NSDictionary<UIApplicationOpenURLOptionsKey, id> *)options;


@end

@interface FoodLens (FoodAR)

/**
    Returns new authorized FoodLens instance.

    Call it for the first time when @c lastAuthorizedInstanceWithCompletion: returns nil. @c lastAuthorizedInstanceWithCompletion: should be called next time.
    @see lastAuthorizedInstanceWithCompletion

    The demoCode should be obtained from the owner.

    @param code A demo code. Obtain it from the owner.
    @param completion A completion. Is called with either authorized FoodLens instance or error.
 */
+ (void)authorizedInstanceWithFoodARDemoCode:(nonnull NSString *)code completion:(nonnull void(^)(FoodLens *_Nullable foodLens, NSError *_Nullable error))completion;


/**
    Instantiates FoodAR camera view controller.

    The resulting view controller initializes the camera and starts live tracking of the captured frames.
    The controller calls delegate when it did find or update food instance.

    @param options Currently should be nil or empty

    @return Returns new view controller instance.
 */
- (nonnull UIViewController *)instantiateFoodARCameraViewControllerWithOptions:(nullable NSDictionary *)options;

/**
    Pauses given view controller.

    As a result view controller stops video capturing and tracking but doesn't reset already discovered category list.
    Can be used to temporary break food discovering e.g. while presenting some overlay UI.
    @c foodARCameraViewControllerResume: resumes video capturing and tracking.
    @c foodARCameraViewControllerRestart: should be used to reset everything including already discovered categories.

    It's not neccessary to call the method when UI disappears. The view controller take care of this on itself.

    @param cameraViewController An instance of FoodAR camera view controller
 */
- (void)foodARCameraViewControllerPause:(nonnull UIViewController *)cameraViewController;

/**
    Resumes given view controller.

    As a result view controller resumes previosuly paused video capturing and tracking and starts reporting to delegate.

    @param cameraViewController An instance of FoodAR camera view controller
 */
- (void)foodARCameraViewControllerResume:(nonnull UIViewController *)cameraViewController;

/**
    Restarts given view controller.

    As a result view controller resets all internal structures and starts as it was right after initialization.
    Restarts video capturing and tracking if view controller was previosly paused with @c foodARCameraViewControllerPause:

    @param cameraViewController An instance of FoodAR camera view controller
 */
- (void)foodARCameraViewControllerRestart:(nonnull UIViewController *)cameraViewController;

/**
    Removes given category from the list of previously discovered and reported.

    As a result view controller forgets given category so that if it is discovered next time then reported as a new.
    In order to wipe them all out use @c foodARCameraViewControllerRestart:

    @param cameraViewController An instance of FoodAR camera view controller
    @param category A category to be removed
 */
- (void)foodARCameraViewController:(nonnull UIViewController *)cameraViewController removeCategory:(nonnull id<FoodLensFoodCategory>)category;

@end
