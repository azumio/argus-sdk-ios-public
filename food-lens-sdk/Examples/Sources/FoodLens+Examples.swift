//
//  FoodLens+Examples.swift
//  AICamera
//
//  Created by Vyacheslav Kuleshov on 5/20/20.
//  Copyright © 2020 Azumio Inc. All rights reserved.
//

import FoodLensSDK

extension FoodLens {

    static let endpoint = URL(string: "https://testjwt-dot-default-dot-azumio-com.appspot.com")!
    static let clientId = "3ffbd6d5"
    static let clientSecret = "7a5514e8b163c984a7bc0c69c5483161"

    private static var urlSession = URLSession(configuration: URLSessionConfiguration.default)

    struct TokenPayload: Encodable {
        var grant_type = "foodapi"
        var client_id = FoodLens.clientId
        var client_secret = FoodLens.clientSecret
        var user_id: String
    }

    struct TokenResponse: Decodable {
        var access_token: String
    }

    struct TokenErrorResponse: Decodable {
        struct Error: Decodable {
            var errorDetail: String
            var code: Int
        }
        var error: Error
    }

    class func authorizedInstance(_ completion: @escaping (FoodLens?, Error?) -> Void) {
        let userId = UUID().uuidString
        var request = URLRequest(url: Self.endpoint.appendingPathComponent("api2/token"))
        request.setValue("application/json", forHTTPHeaderField: "Content-Type")
        request.httpMethod = "POST"
        let payload = TokenPayload(user_id: userId)
        
        let task = urlSession.uploadTask(with: request, from: try! JSONEncoder().encode(payload)) { (responseData, response, error) in
            var errorDescription = error?.localizedDescription
            if let data = responseData {
                do {
                    let response = try JSONDecoder().decode(TokenResponse.self, from: data)
                    FoodLens.authorizedInstance(withAccessToken: response.access_token) { (foodLens, error) in
                        DispatchQueue.main.async {
                            completion(foodLens, error)
                        }
                    }
                } catch (let e) {
                    do {
                        let response = try JSONDecoder().decode(TokenErrorResponse.self, from: data)
                        errorDescription = response.error.errorDetail
                    } catch (_) {
                        errorDescription = e.localizedDescription
                    }
                }
            }
        
            DispatchQueue.main.async {
                if let errorDescription = errorDescription {
                    completion(nil, NSError(domain: "FoodLensExamples", code: 100, userInfo: [NSLocalizedDescriptionKey: errorDescription]))
                }
            }
        }
        task.resume()
    }

}
