# FoodLens SDK

The framework for recognizing food

## Getting Started


### Installing

Using CocoaPods

```
pod 'FoodLensSDK'
```

Will install the SDK as a precompiled static library or framework.

In case if framework is preferable then explicitly reference framework version

```
pod 'FoodLensSDK/Framework'
```

or if static then static

```
pod 'FoodLensSDK/Static'
```

### Start using

For the first time launch you have to obtain access token from the designated web services. And start the SDK like 

```
let foodLens = FoodLens.authorizedInstance(withAccessToken: access_token)
```

Next time you have to obtain the last authorized instance. The instance is preserved between app launches

```
let foodLens = FoodLens.lastAuthorizedInstance()
```

Call logout to clean up the last authorized instances and the session related caches

```
FoodLens.logout()
```


### Data source and delegate 

Always assign delegate in order to handle results from UI.
If you are going to use UI less food recognition method then skip this step.

```
foodLens.delegate = self
```

Also assign data source if you are going to use history UI 

```
foodLens.dataSource = self
```

Implement all required delegate and data source methods.

If you don't want to worry about data handling then just leave it out for FoodLens   
FoodLens will manages your data online.
Instantiate online data source and assign it to dataSource property

```
foodLens.dataSource = foodLens.instantiateOnlineHistoryDataSource
```

In this case your delegate implementation may simply looks like

```
func foodLensPut(_ foodLens: FoodLens, foodCheckIn: FoodLensFoodCheckIn, from viewController: UIViewController?) {
    (foodLens as? FoodLensMutableDataSource)?.foodLensPut(foodLens, checkIn: foodCheckIn)
}

func foodLensDelete(_ foodLens: FoodLens, foodCheckIn: FoodLensFoodCheckIn, from viewController: UIViewController?) {
    (foodLens as? FoodLensMutableDataSource)?.foodLensDelete(foodLens, checkIn: foodCheckIn)
}
```

> :hammer_and_wrench: See History example


There are some cases when you might want to implement data source by yourself  

For example if you want to store history data locally

> :hammer_and_wrench: See LocalHistory example


Or integrate the history into your own data structures


### Obtaining access token 

Example how to obtain access token:

```
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
```

> :hammer_and_wrench: See any example


### Recognizing food

For recognizing food on the given image without UI call the next method on authorized instance of FoodLens

```
foodLens.recognizeFood(image) { (segments, error) in
    ...
}
```
 


### Calling FoodLens UI

#### Camera view controller

The camera view controller helps to make food photo, display and confirm results.

```
foodLens.instantiateCameraViewController()
```

#### Search food view controller

The search food view controller allows to search for food.

```
foodLens.instantiateFoodSearchViewController()
```


#### History view

The history view controller allows to browse entire food history 

```
foodLens.instantiateHistoryViewController()
```


#### Confirm food recognition result

The edit view controller allows to edit and confirm food recognition results for the given image.

```
foodLens.instantiateResultViewController(forImage: image)
```


#### View food recognition result

The view controller allows to view, edit, delete and confirm previously confirmed food checkins.

```
foodLens.instantiateResultViewController(forCheckIn: checkIn)
```




## Examples

Before you start run in Examples folder

```
pod update
```


#### :hammer_and_wrench: AICamera

Demonstrates camera view controller features

#### :hammer_and_wrench: SearchFood

Demonstrates food search view controller features

#### :hammer_and_wrench: History

Demonstrates how to use FoodLens provided online data source

#### :hammer_and_wrench: LocalHistory

Demonstrates how to create your own data source on example of locally stored history

#### :hammer_and_wrench: RecognizeFood

Demonstrates how to use UI less food recognition method for getting food on the picture from the library 

#### :hammer_and_wrench: AICameraWithCustomUI

Demonstrates how to add custom controls inside camera view controller 

#### :hammer_and_wrench: ResultFromPictureLibrary

Demonstrates how to present edit screen for the food recognition result for the given UIImage bypassing camera screen 

#### :hammer_and_wrench: AICameraAndViewResult

Demonstrates how to view, edit and delete previously confirmed food checkins  


## License

TBD

## Copyright

Copyright © 2020 Azumio Inc. All rights reserved.
