#include <fbjni/fbjni.h>
#include <torch/script.h>
#include <torch/csrc/jit/mobile/import.h>
#include <torch/csrc/jit/mobile/module.h>

#ifdef __ANDROID__

#include <android/log.h>

#endif

namespace {

    using namespace facebook;

    static inline void logTensor(std::string label, torch::Tensor &tensor) {
#ifdef __ANDROID__
        std::ostringstream stream;
        stream << "shape: " << tensor.sizes() << "\n dtype: " << tensor.dtype() << "\n data: "
               << tensor;
        std::string tensorString = stream.str();

        __android_log_print(
                ANDROID_LOG_DEBUG,
                "YOLOv5",
                "%s: %s",
                label.c_str(),
                tensorString.c_str());
#endif
    }

    class YOLOv5 : public jni::JavaClass<YOLOv5> {
    public:
        static constexpr auto kJavaDescriptor =
                "Lorg/pytorch/helloworld/YOLOv5;";

        static void registerNatives() {
            javaClassStatic()->registerNatives(
                    {
                            makeNativeMethod("loadAndForward", YOLOv5::loadAndForward)
                    });
        }

    private:
        static void loadAndForward(jni::alias_ref<YOLOv5> clazz, std::string filePath) {

#ifdef __ANDROID__
            __android_log_print(ANDROID_LOG_DEBUG, "YOLOv5", "model file path %s",
                                filePath.c_str());
#endif

            auto tensor = torch::rand({1, 3, 640, 480});
            std::vector<c10::IValue> inputs{tensor};

            logTensor("input tensor", tensor);

            c10::InferenceMode guard;
            auto module_ = torch::jit::_load_for_mobile(filePath, torch::kCPU);
            // Calling the forward will cause a Fatal signal 11 (SIGSEGV), code 1 (SEGV_MAPERR), fault addr 0x71366647e0 in tid 32021 (orch.helloworld), pid 32000 (orch.helloworld)
            module_.forward(inputs);
        }
    };

} // namespace

JNIEXPORT jint

JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    return facebook::jni::initialize(vm, [] { YOLOv5::registerNatives(); });
}

