# Quickstart

[HelloWorld](https://github.com/pytorch/android-demo-app/tree/master/HelloWorldApp) is a simple image classification application that demonstrates how to use PyTorch Android API.
This application runs TorchScript serialized TorchVision pretrained [MobileNet v3 model](https://pytorch.org/vision/stable/models.html) on static image which is packaged inside the app as android asset.


## 1. Model

The TorchScript yolort model is part of the repo in `app/src/main/assets/yolo5s_scriptmodule.ptl`.


## 2. Cloning from GitHub

```
git clone https://github.com/raedle/android-demo-app.git
cd HelloWorldApp
```

## 3. Build and install debug build

If [Android SDK](https://developer.android.com/studio/index.html#command-tools) and [Android NDK](https://developer.android.com/ndk/downloads) are already installed you can install this application to the connected android device or emulator with:

```
./gradlew installDebug
```

We recommend you to open this project in [Android Studio 3.5.1+](https://developer.android.com/studio) (At the moment PyTorch Android and demo applications use [android gradle plugin of version 3.5.0](https://developer.android.com/studio/releases/gradle-plugin#3-5-0), which is supported only by Android Studio version 3.5.1 and higher),
in that case you will be able to install Android NDK and Android SDK using Android Studio UI.


## 4. Gradle dependencies

PyTorch Android is added to the HelloWorld as [gradle dependencies](https://github.com/pytorch/android-demo-app/blob/master/HelloWorldApp/app/build.gradle#L28-L29) in build.gradle:

```
repositories {
    jcenter()
}

dependencies {
    implementation 'org.pytorch:pytorch_android_lite:1.10.0'
    implementation 'org.pytorch:pytorch_android_torchvision_lite:1.10.0'
}
```

Where `org.pytorch:pytorch_android` is the main dependency with PyTorch Android API, including libtorch native library for all 4 android abis (armeabi-v7a, arm64-v8a, x86, x86_64).
Further in this doc you can find how to rebuild it only for specific list of android abis.

`org.pytorch:pytorch_android_torchvision` - additional library with utility functions for converting `android.media.Image` and `android.graphics.Bitmap` to tensors.

The JNI bits of the PyTorch Android dependency will not be used, but instead it extracts the PyTorch C++ frontend API (i.e., `libpytorch_jni_lite.so`) and uses the C++ API (see `src/main/cpp/OnLoad.cpp`).


## 5. Get Crash Report

The app will instacrash on start with the following error:

```
signal 11 (SIGSEGV), code 2 (SEGV_ACCERR), fault addr 0x7ac331b7e0
```

Start logcat to receive crash report

```
adb logcat --buffer=crash
```

Then run the `./gradlew installDebug` command again.


## 6. Get Crash Report

If the model works with PyTorch Mobile, the app should not crash. Additionally, the `module_.forward(inputs)` in `src/main/cpp/OnLoad.cpp` should should return a sensible result.

```
auto output = module_.forward(inputs);
```


## 7. Alternative Model

Check that alternative model works. Go to `app/src/main/java/org/pytorch/helloworld/MainActivity.java` and replace

```
final String filePath = assetFilePath(this, "yolov5s_scriptmodule.ptl");
```

with

```
final String filePath = assetFilePath(this, "mobilenet_v3_small.ptl");
```

Rebuild debug build and install it again with

```
./gradlew installDebug
```

The app will load successfully without crashing
