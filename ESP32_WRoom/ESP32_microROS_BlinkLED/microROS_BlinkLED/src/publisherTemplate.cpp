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
// rcl_publisher_t publisher;
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
// void timer_callback(rcl_timer_t * timer, int64_t last_call_time);

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

//     // Create publisher (std_msgs/Int32)
//     RCCHECK(rclc_publisher_init_default(
//         &publisher,
//         &node,
//         ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
//         "micro_ros_esp32_node_publisher"));

//     // Timer period (1000 ms → publishes at 1 Hz)
//     const unsigned int timer_timeout = 1000;
//     RCCHECK(rclc_timer_init_default(
//         &timer,
//         &support,
//         RCL_MS_TO_NS(timer_timeout),
//         timer_callback));

//     // Create executor (runs callbacks)
//     RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
//     // Attach timer to executor
//     RCCHECK(rclc_executor_add_timer(&executor, &timer));

//     msg.data = 0;
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

// // <---- ----- timer_callback function ----- ---->
// void timer_callback(rcl_timer_t * timer, int64_t last_call_time)
// {  
//     RCLC_UNUSED(last_call_time);

//     if(timer != NULL)
//     {
//         // Publish the message, but don't crash if agent disconnected
//         RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
//         msg.data++;
//     }
// }

