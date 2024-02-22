^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package usb_cam
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

0.3.6 (2017-06-15)
------------------
* .travis.yml: udpate to trusty
* add AV\_ prefix to PIX_FMT\_* for X,Y (`#71 <https://github.com/ros-drivers/usb_cam/issues/71>`_)
* Contributors: Kei Okada

0.3.5 (2017-06-14)
------------------
* add ROS Orphaned Package Maintainers to maintainer tag (`#69 <https://github.com/ros-drivers/usb_cam/issues/69>`_)
* support for Kinetic / Ubuntu 16.04 (`#58 <https://github.com/ros-drivers/usb_cam/issues/58>`_)
  * replace use of deprecated functions in newer ffmpeg/libav versions
    ffmpeg/libav 55.x (used in ROS Kinetic) deprecated the avcodec_alloc_frame.
* Add grey scale pixel format. (`#45 <https://github.com/ros-drivers/usb_cam/issues/45>`_)
* add start/stop capture services (`#44 <https://github.com/ros-drivers/usb_cam/issues/44>`_ )
  * better management of start/stop
  * up package.xml
  * add capture service

* fix bug for byte count in a pixel (3 bytes not 24 bytes) (`#40 <https://github.com/ros-drivers/usb_cam/issues/40>`_ )
* Contributors: Daniel Seifert, Eric Zavesky, Kei Okada, Ludovico Russo, Russell Toris, honeytrap15

0.3.4 (2015-08-18)
------------------
* Installs launch files
* Merge pull request #37 from tzutalin/develop
  Add a launch file for easy test
* Add a launch file for easy test
* Contributors: Russell Toris, tzu.ta.lin

0.3.3 (2015-05-14)
------------------
* Merge pull request #36 from jsarrett/develop
  add gain parameter
* add gain parameter
* Contributors: James Sarrett, Russell Toris

0.3.2 (2015-03-24)
------------------
* Merge pull request #34 from eliasm/develop
  fixed check whether calibration file exists
* fixed check whether calibration file exists
* Contributors: Elias Mueggler, Russell Toris

0.3.1 (2015-02-20)
------------------
* Merge pull request #32 from kmhallen/mono8
  Publish YUVMONO10 images as mono8 instead of rgb8
* Publish YUVMONO10 images as mono8 instead of rgb8
* Contributors: Kevin Hallenbeck, Russell Toris

0.3.0 (2015-01-26)
------------------
* Merge pull request #30 from mitchellwills/develop
  Removed global state from usb_cam by encapsulating it inside an object
* Made device name a std::string instead of const char*
* Added usb_cam namespace
* Added underscore sufix to class fields
* Removed camera_ prefix from methods
* Moved methods to parse pixel_format and io_method from string to UsbCam
* Moved camera_image_t struct to be private in UsbCam
* Cleaned up parameter assignment
* Made set_v4l_parameters a non-static function
* Moved set_v4l_parameters to UsbCam object
* Removed global state from usb_cam by encapsulating it inside an object
  function and structions in usb_cam.h became public and everything else is private
* Merge pull request #28 from mitchellwills/develop
  Fix installation of header files
* Fix installation of header files
* Contributors: Mitchell Wills, Russell Toris

0.2.0 (2015-01-16)
------------------
* Bug fix in camera info settings.
* Update .travis.yml
* Merge pull request #27 from bosch-ros-pkg/default_camera_info
  sets default camera info
* sets default camera info
* Contributors: Russell Toris

0.1.13 (2014-12-02)
-------------------
* Merge pull request #25 from blutack/patch-1
  Warn rather than error if framerate can't be set
* Warn rather than error if framerate can't be set
  The driver doesn't currently work with em28xx based devices as they don't allow the framerate to be set directly and the node exits with an error. Changing to a warning allows these devices to be used.
* Update README.md
* Merge pull request #24 from rjw57/do-not-touch-parameters-unless-asked
  do not modify parameters unless explicitly set
* do not modify parameters unless explicitly set
  The contrast, saturation, brightness, sharpness and focus parameters
  were recently added to usb_cam. This caused a regression
  (sigproc/robotic_surgery#17) whereby the default settings for a webcam
  are overridden in all cases by the hard-coded defaults in usb_cam.
  In the absence of a know good set of "default" values, leave the
  parameters unset unless the user has explicitly set them in the launch
  file.
* Contributors: Rich Wareham, Russell Toris, blutack

0.1.12 (2014-11-05)
-------------------
* Merge pull request #22 from dekent/develop
  White balance parameters
* Parameter to enable/disable auto white balance
* Added parameters for white balance
* uses version major to check for av_codec
* uses version header to check for AV_CODEC_ID_MJPEG
* Contributors: David Kent, Russell Toris

0.1.11 (2014-10-30)
-------------------
* Merge pull request #20 from dekent/develop
  More Parameters
* bug fix
* Setting focus when autofocus is disabled
* Parameter adjusting
* Added parameter setting for absolute focus, brightness, contrast, saturation, and sharpness
* Contributors: David Kent, Russell Toris

0.1.10 (2014-10-24)
-------------------
* Merge pull request #19 from bosch-ros-pkg/av_codec_id
  Removed deprecated CODEC_ID
* added legacy macro constants for libav 10
* Renamed deprecated CODEC_ID constants to AV_CODEC_ID to fix compilation for libav 10
* Contributors: Andrzej Pronobis, Russell Toris

0.1.9 (2014-08-26)
------------------
* Uses ros::Rate to enforce software framerate instead of custom time check
* Merge pull request #16 from liangfok/feature/app_level_framerate_control
  Modified to enforce framerate control at the application level in additi...
* Modified to enforce framerate control at the application level in addition to at the driver level.  This is necessary since the drivers for my webcam did not obey the requested framerate.
* Contributors: Russell Toris, liang

0.1.8 (2014-08-21)
------------------
* autoexposure and exposure settings now exposed via ROS parameters
* added ability to call v4l-utils as well as correctly set autofocus
* cleanup of output
* Merge pull request #15 from mistoll/develop
  added support for RGB24 pixel format
* Added RGB24 as pixel format
* Contributors: Michael Stoll, Russell Toris

0.1.7 (2014-08-20)
------------------
* changelog fixed
* minor cleanup and ability to change camera name and info
* Contributors: Russell Toris

0.1.6 (2014-08-15)
------------------
* Merge pull request #14 from KaijenHsiao/master
  added support for 10-bit mono cameras advertising as YUV
* added support for 10-bit mono cameras advertising as YUV (such as Leopard Imaging's LI-USB30-V034)
* Update CHANGELOG.rst
* changelog updated
* Merge pull request #13 from vrabaud/develop
  add a a ros::spinOnce to get set_camera_info working
* add a a ros::spinOnce to get set_camera_info working
  This is explained in the docs of CameraInfoManager
  https://github.com/ros-perception/image_common/blob/hydro-devel/camera_info_manager/include/camera_info_manager/camera_info_manager.h#L71
  Also, this fixes https://github.com/ros-perception/image_pipeline/issues/78
* Contributors: Kaijen Hsiao, Russell Toris, Vincent Rabaud, sosentos

0.1.5 (2014-07-28)
------------------
* auto format
* cleanup of readme and such
* Merge branch 'hydro-devel' of github.com:bosch-ros-pkg/usb_cam
* Merge pull request #11 from pronobis/hydro-devel
  Fixed a bug with av_free missing by adding a proper include.
* Fixed a bug with av_free missing by adding a proper include on Ubuntu 14.04.
* Merge pull request #7 from cottsay/groovy-devel
  Use pkg-config to find avcodec and swscale
* Merge pull request #5 from FriedCircuit