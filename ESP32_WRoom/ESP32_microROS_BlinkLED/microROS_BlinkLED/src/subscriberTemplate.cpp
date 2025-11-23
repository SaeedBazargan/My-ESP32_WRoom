// #include <Arduino.h>
// #include <micro_ros_platformio.h>

// #include <stdio.h>
// #include <rcl/rcl.h>
// #include <rcl/error_handling.h>
// #include <rclc/rclc.h>
// #include <rclc/executor.h>

// #include <std_msgs/msg/int32.h>

// // --------------------------------------------------
// // <---- ----- Global variables ----- ---->
// // These are required by the micro-ROS stack.
// // Avoid putting dynamic allocations in loop().
// // --------------------------------------------------
// rcl_subscription_t subscriber;
// std_msgs__msg__Int32 msg;
// rclc_executor_t executor;
// rclc_support_t support;
// rcl_allocator_t allocator;
// rcl_node_t node;
// rcl_timer_t timer;

// // --------------------------------------------------
// // <---- ----- Defines ----- ---->
// // Hard check: stops the system on failure
// // --------------------------------------------------
// #define LED_PIN 4

// #define RCCHECK(fn){                        \
//     rcl_ret_t temp_rc = fn;                 \
//     if(temp_rc != RCL_RET_OK)               \
//     {                                       \
//         error_loop();                       \
//     }                                       \
// }

// // Soft check: ignores error but prevents crash
// #define RCSOFTCHECK(fn){                    \
//     rcl_ret_t temp_rc = fn;                 \
//     if(temp_rc != RCL_RET_OK)               \
//     {/* optional: add debug print */}       \
// }

// // <---- ----- Functions ----- ---->
// void error_loop();
// void subscription_callback(const void * msg_in);

// // <---- ----- Int main function ----- ---->
// void setup()
// {
//     Serial.begin(115200);
//     pinMode(LED_PIN, OUTPUT);

//     // Select micro-ROS transport. Must be called before initialization
//     set_microros_serial_transports(Serial);
    
//     delay(2000);

//     allocator = rcl_get_default_allocator();

//     // create init_options
//     // Initialize micro-ROS support structure (context + options)
//     RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

//     // create node
//     RCCHECK(rclc_node_init_default(&node, "micro_ros_esp32_node", "", &support));

//     // Create subscriber (std_msgs/Int32)
//     RCCHECK(rclc_subscription_init_default(
//         &subscriber,
//         &node,
//         ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
//         "micro_ros_esp32_subscriber"));

//     // Create executor (runs callbacks)
//     RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
//     // Attach subscription to executor
//     RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &msg, &subscription_callback, ON_NEW_DATA));
// }

// // <---- ----- main loop function ----- ---->
// void loop()
// {
//     delay(100);
//     // Process micro-ROS callbacks (non-blocking)
//     RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
// }

// // --------------------------------------------------
// // <---- ----- Error_handler function (blocking) ----- ---->
// // If a critical failure happens (invalid init, lost memory, etc)
// // system enters a safe infinite loop and blinks LED.
// // --------------------------------------------------
// void error_loop()
// {
//     while(1)
//     {
//         digitalWrite(LED_PIN, !digitalRead(LED_PIN));
//         delay(250);
//     }
// }


// void subscription_callback(const void * msg_in)
// {
//     const std_msgs__msg__Int32 * msg = (const std_msgs__msg__Int32 *)msg_in;
//     digitalWrite(LED_PIN, (msg->data == 0) ? LOW : HIGH);
// }