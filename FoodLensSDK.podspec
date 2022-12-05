#
# Be sure to run `pod lib lint FoodLensSDK.podspec' to ensure this is a
# valid spec before submitting.
#
# Any lines starting with a # are optional, but their use is encouraged
# To learn more about a Podspec see https://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|
  s.name             = 'FoodLensSDK'
  s.version          = '0.3.17'
  s.summary          = 'FoodLens SDK as a prebuilt framework'
  s.description      = 'FoodLens SDK as a prebuilt framework'

  s.homepage         = 'https://github.com/azumio/argus-sdk-ios-public/food-lens-sdk'
  s.license          = { :type => 'Azumio Developer License', :file => 'LICENSE' }
  s.author           = { 'Azumio Inc.' => 'info@azumio.com' }
  s.source           = { :git => 'https://github.com/azumio/argus-sdk-ios-public.git', :tag => 'v' + s.version.to_s  }

  s.ios.deployment_target = '11.0'
  s.requires_arc = true
  s.swift_versions = '5.0'

  s.module_name = 'FoodLensSDK'

  s.subspec 'Static' do |subs|
      subs.dependency 'FoodLensSDKXCFrameworkStatic'
  end

  s.subspec 'Framework' do |subs|
      subs.dependency 'FoodLensSDKXCFramework'
  end

  s.default_subspecs = 'Static'

end
