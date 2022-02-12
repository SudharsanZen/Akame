#pragma once
#include"Core/AkameCore.h"
/*! @} */

/*! @brief One.
 *
 *  This is only semantic sugar for the number 1.  You can instead use `1` or
 *  `true` or `_True` or `GL_TRUE` or `VK_TRUE` or anything else that is equal
 *  to one.
 *
 *  @ingroup init
 */
#define TRUE                   1
 /*! @brief Zero.
  *
  *  This is only semantic sugar for the number 0.  You can instead use `0` or
  *  `false` or `_False` or `GL_FALSE` or `VK_FALSE` or anything else that is
  *  equal to zero.
  *
  *  @ingroup init
  */
#define FALSE                  0

  /*! @name Key and button actions
   *  @{ */
   /*! @brief The key or mouse button was released.
	*
	*  The key or mouse button was released.
	*
	*  @ingroup input
	*/
#define RELEASE                0
	/*! @brief The key or mouse button was pressed.
	 *
	 *  The key or mouse button was pressed.
	 *
	 *  @ingroup input
	 */
#define PRESS                  1
	 /*! @brief The key was held down until it repeated.
	  *
	  *  The key was held down until it repeated.
	  *
	  *  @ingroup input
	  */
#define REPEAT                 2
	  /*! @} */

	  /*! @defgroup hat_state Joystick hat states
	   *  @brief Joystick hat states.
	   *
	   *  See [joystick hat input](@ref joystick_hat) for how these are used.
	   *
	   *  @ingroup input
	   *  @{ */
#define HAT_CENTERED           0
#define HAT_UP                 1
#define HAT_RIGHT              2
#define HAT_DOWN               4
#define HAT_LEFT               8
#define HAT_RIGHT_UP           (GLFW_HAT_RIGHT | GLFW_HAT_UP)
#define HAT_RIGHT_DOWN         (GLFW_HAT_RIGHT | GLFW_HAT_DOWN)
#define HAT_LEFT_UP            (GLFW_HAT_LEFT  | GLFW_HAT_UP)
#define HAT_LEFT_DOWN          (GLFW_HAT_LEFT  | GLFW_HAT_DOWN)
	   /*! @} */

	   /*! @defgroup keys Keyboard keys
		*  @brief Keyboard key IDs.
		*
		*  See [key input](@ref input_key) for how these are used.
		*
		*  These key codes are inspired by the _USB HID Usage Tables v1.12_ (p. 53-60),
		*  but re-arranged to map to 7-bit ASCII for printable keys (function keys are
		*  put in the 256+ range).
		*
		*  The naming of the key codes follow these rules:
		*   - The US keyboard layout is used
		*   - Names of printable alpha-numeric characters are used (e.g. "A", "R",
		*     "3", etc.)
		*   - For non-alphanumeric characters, Unicode:ish names are used (e.g.
		*     "COMMA", "LEFT_SQUARE_BRACKET", etc.). Note that some names do not
		*     correspond to the Unicode standard (usually for brevity)
		*   - Keys that lack a clear US mapping are named "WORLD_x"
		*   - For non-printable keys, custom names are used (e.g. "F4",
		*     "BACKSPACE", etc.)
		*
		*  @ingroup input
		*  @{
		*/

		/* The unknown key */
#define GLFW_KEY_UNKNOWN            -1

/* Printable keys */
#define KEY_SPACE              32
#define KEY_APOSTROPHE         39  /* ' */
#define KEY_COMMA              44  /* , */
#define KEY_MINUS              45  /* - */
#define KEY_PERIOD             46  /* . */
#define KEY_SLASH              47  /* / */
#define KEY_0                  48
#define KEY_1                  49
#define KEY_2                  50
#define KEY_3                  51
#define KEY_4                  52
#define KEY_5                  53
#define KEY_6                  54
#define KEY_7                  55
#define KEY_8                  56
#define KEY_9                  57
#define KEY_SEMICOLON          59  /* ; */
#define KEY_EQUAL              61  /* = */
#define KEY_A                  65
#define KEY_B                  66
#define KEY_C                  67
#define KEY_D                  68
#define KEY_E                  69
#define KEY_F                  70
#define KEY_G                  71
#define KEY_H                  72
#define KEY_I                  73
#define KEY_J                  74
#define KEY_K                  75
#define KEY_L                  76
#define KEY_M                  77
#define KEY_N                  78
#define KEY_O                  79
#define KEY_P                  80
#define KEY_Q                  81
#define KEY_R                  82
#define KEY_S                  83
#define KEY_T                  84
#define KEY_U                  85
#define KEY_V                  86
#define KEY_W                  87
#define KEY_X                  88
#define KEY_Y                  89
#define KEY_Z                  90
#define KEY_LEFT_BRACKET       91  /* [ */
#define KEY_BACKSLASH          92  /* \ */
#define KEY_RIGHT_BRACKET      93  /* ] */
#define KEY_GRAVE_ACCENT       96  /* ` */
#define KEY_WORLD_1            161 /* non-US #1 */
#define KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define KEY_ESCAPE             256
#define KEY_ENTER              257
#define KEY_TAB                258
#define KEY_BACKSPACE          259
#define KEY_INSERT             260
#define KEY_DELETE             261
#define KEY_RIGHT              262
#define KEY_LEFT               263
#define KEY_DOWN               264
#define KEY_UP                 265
#define KEY_PAGE_UP            266
#define KEY_PAGE_DOWN          267
#define KEY_HOME               268
#define KEY_END                269
#define KEY_CAPS_LOCK          280
#define KEY_SCROLL_LOCK        281
#define KEY_NUM_LOCK           282
#define KEY_PRINT_SCREEN       283
#define KEY_PAUSE              284
#define KEY_F1                 290
#define KEY_F2                 291
#define KEY_F3                 292
#define KEY_F4                 293
#define KEY_F5                 294
#define KEY_F6                 295
#define KEY_F7                 296
#define KEY_F8                 297
#define KEY_F9                 298
#define KEY_F10                299
#define KEY_F11                300
#define KEY_F12                301
#define KEY_F13                302
#define KEY_F14                303
#define KEY_F15                304
#define KEY_F16                305
#define KEY_F17                306
#define KEY_F18                307
#define KEY_F19                308
#define KEY_F20                309
#define KEY_F21                310
#define KEY_F22                311
#define KEY_F23                312
#define KEY_F24                313
#define KEY_F25                314
#define KEY_KP_0               320
#define KEY_KP_1               321
#define KEY_KP_2               322
#define KEY_KP_3               323
#define KEY_KP_4               324
#define KEY_KP_5               325
#define KEY_KP_6               326
#define KEY_KP_7               327
#define KEY_KP_8               328
#define KEY_KP_9               329
#define KEY_KP_DECIMAL         330
#define KEY_KP_DIVIDE          331
#define KEY_KP_MULTIPLY        332
#define KEY_KP_SUBTRACT        333
#define KEY_KP_ADD             334
#define KEY_KP_ENTER           335
#define KEY_KP_EQUAL           336
#define KEY_LEFT_SHIFT         340
#define KEY_LEFT_CONTROL       341
#define KEY_LEFT_ALT           342
#define KEY_LEFT_SUPER         343
#define KEY_RIGHT_SHIFT        344
#define KEY_RIGHT_CONTROL      345
#define KEY_RIGHT_ALT          346
#define KEY_RIGHT_SUPER        347
#define KEY_MENU               348

#define KEY_LAST               GLFW_KEY_MENU

	   /*! @} */

	   /*! @defgroup buttons Mouse buttons
		*  @brief Mouse button IDs.
		*
		*  See [mouse button input](@ref input_mouse_button) for how these are used.
		*
		*  @ingroup input
		*  @{ */
#define MOUSE_BUTTON_1         0
#define MOUSE_BUTTON_2         1
#define MOUSE_BUTTON_3         2
#define MOUSE_BUTTON_4         3
#define MOUSE_BUTTON_5         4
#define MOUSE_BUTTON_6         5
#define MOUSE_BUTTON_7         6
#define MOUSE_BUTTON_8         7
#define MOUSE_BUTTON_LAST      MOUSE_BUTTON_8
#define MOUSE_BUTTON_LEFT      MOUSE_BUTTON_1
#define MOUSE_BUTTON_RIGHT     MOUSE_BUTTON_2
#define MOUSE_BUTTON_MIDDLE    MOUSE_BUTTON_3
		/*! @} */



struct GLFWwindow;
class AKAME_API Input
{
private:
	class keyState
	{
	public:
		int currentState=0;
		int oldState=0;

		bool up=false;
		bool down=false;

	};
	static keyState keyPressed[1024];
	
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	friend class Window;
	friend class Scene;

	static void flush();
public:
	
	
	static bool getKeyDown(int key);
	static bool getKeyUp(int key);
	static bool getKey(int key);

	static void getMouseXY(double &x,double &y);
	static int getMouseButton(int button);

};

