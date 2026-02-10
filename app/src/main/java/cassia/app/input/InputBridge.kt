package cassia.app.input

object InputBridge {
    init {
        try {
            System.loadLibrary("cassia")
        } catch (_: Throwable) {
        }
    }

    @JvmStatic
    external fun pointerEvent(pointerId: Int, x: Double, y: Double, action: Int)

    @JvmStatic
    external fun keyEvent(scanCode: Int, down: Boolean)
}
