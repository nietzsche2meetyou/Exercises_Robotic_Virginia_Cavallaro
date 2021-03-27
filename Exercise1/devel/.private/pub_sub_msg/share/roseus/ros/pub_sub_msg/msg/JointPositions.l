;; Auto-generated. Do not edit!


(when (boundp 'pub_sub_msg::JointPositions)
  (if (not (find-package "PUB_SUB_MSG"))
    (make-package "PUB_SUB_MSG"))
  (shadow 'JointPositions (find-package "PUB_SUB_MSG")))
(unless (find-package "PUB_SUB_MSG::JOINTPOSITIONS")
  (make-package "PUB_SUB_MSG::JOINTPOSITIONS"))

(in-package "ROS")
;;//! \htmlinclude JointPositions.msg.html


(defclass pub_sub_msg::JointPositions
  :super ros::object
  :slots (_positions ))

(defmethod pub_sub_msg::JointPositions
  (:init
   (&key
    ((:positions __positions) (make-array 0 :initial-element 0.0 :element-type :float))
    )
   (send-super :init)
   (setq _positions __positions)
   self)
  (:positions
   (&optional __positions)
   (if __positions (setq _positions __positions)) _positions)
  (:serialization-length
   ()
   (+
    ;; float64[] _positions
    (* 8    (length _positions)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64[] _positions
     (write-long (length _positions) s)
     (dotimes (i (length _positions))
       (sys::poke (elt _positions i) (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64[] _positions
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _positions (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _positions i) (sys::peek buf ptr- :double)) (incf ptr- 8)
     ))
   ;;
   self)
  )

(setf (get pub_sub_msg::JointPositions :md5sum-) "3b33cf2d1f9005e741ee291d519b97ee")
(setf (get pub_sub_msg::JointPositions :datatype-) "pub_sub_msg/JointPositions")
(setf (get pub_sub_msg::JointPositions :definition-)
      "float64[] positions
")



(provide :pub_sub_msg/JointPositions "3b33cf2d1f9005e741ee291d519b97ee")


