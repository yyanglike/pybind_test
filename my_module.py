# my_module.py

import threading
import time

# 定义一个全局字典用于存储数据
data_dict = {}

# 定义一个全局锁，用于保护对数据字典的访问
data_lock = threading.Lock()
# Python函数，用于设置回调函数
def set_callback(callback):
    global data_callback_1
    data_callback_1 = callback

# Python函数，用于接收C++传递的数据
def receive_data(data):

    while True:
        # 获取锁
        data_lock.acquire()
        try:
            data_dict['data'] = data
        finally:
            # 释放锁
            data_lock.release()

        print("Python thread received data:", data)

        # 调用回调函数，处理数据
        # global data_callback_1
        # if data_callback_1:
        #     data_callback_1(data)
        
        time.sleep(2)


# Python函数，用于模拟数据生成
def generate_data():
    data = [1, 2, 3, 4, 5]
    receive_data(11)

def main():
    # 启动一个Python线程来模拟数据生成
    data_thread = threading.Thread(target=generate_data)
    data_thread.start()
    data_thread.join()