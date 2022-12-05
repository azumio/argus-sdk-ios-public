#
# Be sure to run `pod lib lint FoodLensSDKXCFrameworkStatic.podspec' to ensure this is a
# valid spec before submitting.
#
# Any lines starting with a # are optional, but their use is encouraged
# To learn more about a Podspec see https://guides.cocoapods.org/syntax/podspec.html
#

#pod_name = 'FoodLensSDKXCFrameworkStatic'
#pod_version = '1.0.77'
#pod_description = 'FoodLens SDK as a prebuilt static xcframework'
#pod_module_name = 'FoodLensSDK'
#pod_module_root = 'FoodLensSDKXCFrameworkStatic/FoodLensSDK.xcframework'
#pod_module_git_url = 'git@github.com:azumio/argus-sdk-ios.git'
#pod_tag_prefix = 'prebuild-foodlenssdk-'
#pod_home_page = 'https://github.com/azumio/argus-sdk-ios'
#pod_deployment_target = '12.0'
#pod_swift_version = '5.0'
#pod_frameworks = 'UIKit'
#pod_author = 'Azumio Inc.'
#pod_author_email = 'info@azumio.com'
#pod_dependencies = 'OpenUDID,Reachability,SSZipArchive'.split(",")
#
#
#Pod::Spec.new do |s|
#  s.name             = pod_name
#  s.version          = pod_version
#  s.summary          = pod_description
#  s.description      = pod_description
#
#  s.homepage         = pod_home_page
#  s.license          = { :type => 'MIT', :file => 'LICENSE' }
#  s.author           = { pod_author => pod_author_email }
#  s.source           = { :git => pod_module_git_url, :tag => pod_tag_prefix + s.version.to_s }
#
#  s.ios.deployment_target = pod_deployment_target
#  s.requires_arc = true
#  s.swift_versions = pod_swift_version
#
#  s.module_name = pod_module_name
#
#  s.source_files = pod_module_root + '/**/Headers/*.h'
#  s.public_header_files = pod_module_root + '/**/Headers/*.h'
#  s.resource = pod_module_root + '/Resources/*'
#  s.vendored_frameworks = pod_module_root
#
#  s.frameworks = pod_frameworks
#  for dependency in pod_dependencies
#    s.dependency dependency
#  end
#
#  s.pod_target_xcconfig = {
#        'HEADER_SEARCH_PATHS' => '"${PODS_XCFRAMEWORKS_BUILD_DIR}"',
#        'OTHER_LDFLAGS' => '$(inherited) -force_load "$(PODS_TARGET_SRCROOT)/' + pod_module_root + '/lib' + pod_module_name + '.a"'
#    }
#
#  s.user_target_xcconfig = {
#        'HEADER_SEARCH_PATHS' => '"${PODS_XCFRAMEWORKS_BUILD_DIR}"',
#        'OTHER_LDFLAGS' => '-force_load "${PODS_XCFRAMEWORKS_BUILD_DIR}/' + pod_name + '/lib' + pod_module_name + '.a"'
#    }
#
#end


#
# Be sure to run `pod lib lint FoodLensSDKXCFrameworkStatic.podspec' to ensure this is a
# valid spec before submitting.
#
# Any lines starting with a # are optional, but their use is encouraged
# To learn more about a Podspec see https://guides.cocoapods.org/syntax/podspec.html
#

pod_name = 'FoodLensSDKXCFrameworkStatic'
pod_version = '0.3.17'
pod_description = 'FoodLens SDK as a prebuilt static xcframework'
pod_module_name = 'FoodLensSDK'
pod_module_root = 'food-lens-sdk/FoodLensSDKXCFrameworkStatic/FoodLensSDK.xcframework'
pod_module_git_url = 'https://github.com/azumio/argus-sdk-ios-public.git'
pod_tag_prefix = 'v'
pod_home_page = 'https://github.com/azumio/argus-sdk-ios-public/food-lens-sdk'
pod_deployment_target = '11.0'
pod_swift_version = '5.0'
pod_frameworks = 'UIKit'
pod_author = 'Azumio Inc.'
pod_author_email = 'info@azumio.com'
pod_dependencies = 'OpenUDID,Reachability,SSZipArchive'.split(",")


Pod::Spec.new do |s|
  s.name             = pod_name
  s.version          = pod_version
  s.summary          = pod_description
  s.description      = pod_description

  s.homepage         = pod_home_page
  s.license          = { :type => 'MIT', :file => 'LICENSE' }
  s.author           = { pod_author => pod_author_email }
  s.source           = { :git => pod_module_git_url, :tag => pod_tag_prefix + s.version.to_s }

  s.ios.deployment_target = pod_deployment_target
  s.requires_arc = true
  s.swift_versions = pod_swift_version

  s.module_name = pod_module_name

  s.source_files = pod_module_root + '/**/Headers/*.h'
  s.public_header_files = pod_module_root + '/**/Headers/*.h'
  s.resource = pod_module_root + '/Resources/*'
  s.vendored_frameworks = pod_module_root

  s.frameworks = pod_frameworks
  for dependency in pod_dependencies
    s.dependency dependency
  end

  s.pod_target_xcconfig = {
        'HEADER_SEARCH_PATHS' => '"${PODS_XCFRAMEWORKS_BUILD_DIR}"',
        'OTHER_LDFLAGS' => '$(inherited) -force_load "$(PODS_TARGET_SRCROOT)/' + pod_module_root + '/lib' + pod_module_name + '.a"'
    }

  s.user_target_xcconfig = {
        'HEADER_SEARCH_PATHS' => '"${PODS_XCFRAMEWORKS_BUILD_DIR}"',
        'OTHER_LDFLAGS' => '-force_load "${PODS_XCFRAMEWORKS_BUILD_DIR}/' + pod_name + '/lib' + pod_module_name + '.a"'
    }

end
