;; Auto-generated. Do not edit!


(when (boundp 'opencvyi::position)
  (if (not (find-package "OPENCVYI"))
    (make-package "OPENCVYI"))
  (shadow 'position (find-package "OPENCVYI")))
(unless (find-package "OPENCVYI::POSITION")
  (make-package "OPENCVYI::POSITION"))

(in-package "ROS")
;;//! \htmlinclude position.msg.html


(defclass opencvyi::position
  :super ros::object
  :slots (_x _y _z _Flag ))

(defmethod opencvyi::position
  (:init
   (&key
    ((:x __x) 0.0)
    ((:y __y) 0.0)
    ((:z __z) 0.0)
    ((:Flag __Flag) 0)
    )
   (send-super :init)
   (setq _x (float __x))
   (setq _y (float __y))
   (setq _z (float __z))
   (setq _Flag (round __Flag))
   self)
  (:x
   (&optional __x)
   (if __x (setq _x __x)) _x)
  (:y
   (&optional __y)
   (if __y (setq _y __y)) _y)
  (:z
   (&optional __z)
   (if __z (setq _z __z)) _z)
  (:Flag
   (&optional __Flag)
   (if __Flag (setq _Flag __Flag)) _Flag)
  (:serialization-length
   ()
   (+
    ;; float64 _x
    8
    ;; float64 _y
    8
    ;; float64 _z
    8
    ;; int32 _Flag
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _x
       (sys::poke _x (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _y
       (sys::poke _y (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _z
       (sys::poke _z (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; int32 _Flag
       (write-long _Flag s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _x
     (setq _x (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _y
     (setq _y (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _z
     (setq _z (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; int32 _Flag
     (setq _Flag (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get opencvyi::position :md5sum-) "0ae3a5e6ef89db379797fe438faae530")
(setf (get opencvyi::position :datatype-) "opencvyi/position")
(setf (get opencvyi::position :definition-)
      "float64 x
float64 y
float64 z
int32 Flag

")



(provide :opencvyi/position "0ae3a5e6ef89db379797fe438faae530")


