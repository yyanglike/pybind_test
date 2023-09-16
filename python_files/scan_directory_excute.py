
import os
import sys
import importlib
import inspect

# 指定目录路径
directory_path = "/path/to/your/directory"

# 添加自定义模块所在的目录到sys.path中
sys.path.append(directory_path)

# 获取目录下所有Python文件的文件名
python_files = [f for f in os.listdir(directory_path) if f.endswith(".py")]

# 遍历每个Python文件
for python_file in python_files:
    module_name = os.path.splitext(python_file)[0]  # 获取模块名称

    try:
        # 动态导入模块
        module = importlib.import_module(module_name)

        # 遍历模块中的成员，查找类
        for name, obj in inspect.getmembers(module):
            if inspect.isclass(obj):
                # 检查类是否包含run方法
                if hasattr(obj, "run") and callable(getattr(obj, "run")):
                    # 实例化类并执行run方法
                    instance = obj()
                    instance.run()
    except ImportError as e:
        print(f"Failed to import module {module_name}: {e}")
    except Exception as e:
        print(f"Error while working with module {module_name}: {e}")
