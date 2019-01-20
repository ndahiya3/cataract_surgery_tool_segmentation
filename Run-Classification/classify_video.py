import tensorflow as tf
import numpy as np
import cv2
import os
import timeit
import fnmatch

# Loads label file, strips off carriage return
def load_labels(label_file):
    label = []
    proto_as_ascii_lines = tf.gfile.GFile(label_file).readlines()
    for l in proto_as_ascii_lines:
        label.append(l.rstrip())
    return label
  
def read_frame(i):
    # read frames_dir/i.jpeg
    file_name = frames_dir + '/' + str(i) + '.jpeg'
    return cv2.imread(file_name)

def read_tensor_from_image_file(file_name):
    input_name = "file_reader"
    output_name = "normalized"
    file_reader = tf.read_file(file_name,input_name)
    image_reader = tf.image.decode_jpeg(file_reader, channels=3,
                                        name='jpeg_reader')
    float_caster = tf.cast(image_reader, tf.float32)
    dims_expander = tf.expand_dims(float_caster, 0);
    resized = tf.image.resize_bilinear(dims_expander, [299,299])
    normalized = tf.divide(resized, [255])
    sess = tf.Session()
    result = sess.run(normalized)

    return result

def load_graph(model_file):
    graph = tf.Graph()
    graph_def = tf.GraphDef()

    with open(model_file, "rb") as f:
        graph_def.ParseFromString(f.read())
    with graph.as_default():
        tf.import_graph_def(graph_def)

    return graph
    
if __name__ == "__main__":
    input_layer = "Mul:0"
    output_layer = "final_result:0"
    
    frames_dir = '/home/ndahiya/frames'
    trained_graph = '/tmp/output_graph.pb'
    num_frames = len(fnmatch.filter(os.listdir(frames_dir),'*.jpeg'))
    label_file = '/tmp/output_labels.txt'
    
    graph = load_graph(trained_graph)
    
    input_name = "import/" + input_layer
    output_name = "import/" + output_layer
    input_operation = graph.get_operation_by_name(input_name);
    output_operation = graph.get_operation_by_name(output_name);
    
    labels = load_labels(label_file)
    
    with tf.Session(graph=graph) as sess:
        for i in range(1,num_frames):
            file_name = frames_dir + '/' + str(i) + '.jpeg'
            t = read_tensor_from_image_file(file_name)
            results = sess.run(output_operation.outputs[0],
                      {input_operation.outputs[0]: t})
            results=np.squeeze(results)
        
            top_k = results.argsort()[-5:][::-1]
            for j in top_k:
                print(labels[j], results[j])
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
