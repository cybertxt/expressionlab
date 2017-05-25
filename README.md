# expressionlab

* 目前属于试验阶段
* 专用于四则运算随机出题【Java】的 **去重** 和 **耗时** 检查

## 输入
通过命令行参数形式指定题目要求

## 输出
输出题目到文件，一行一个题目

## 输入示例
```
$ java ExpressionGenerator 10000 5 expressions.txt
```

* ```ExpressionGenerator``` 为最终执行的类（包含main函数）
* 第一个参数 ```10000``` 代表需要生成的题目数量
* 第二个参数 ```5``` 代表每个题目需包含的运算符数量（除括号以外的运算符数量）
* 第三个参数 ```expressions.txt``` 代表存放生成题目的文件名

## 输出示例
查看 expressions.txt，部分内容如下：
```
...
7 - 8 * 9 / 1 + 2 - 3
4 * 5 / 6 + 7 - 8 * 9
1 / 2 + 3 - 4 * 5 / 6
7 / 8 + 9 - 1 * 2 / 3
...
```

## 特别说明
* 可执行的入口类必须为 ```ExpressionGenerator```
* 参数顺序必须遵守以上 **输入示例** 说明
* 输出格式比较遵守以上 **输出示例** 说明
* 为了减小随机空间，体现出去重功能：
    - 操作数为个位整数，即选择范围只能是：```1```,```2```,```3```,```4```,```5```,```6```,```7```,```8```,```9```这9个数字
    - 操作符选择范围只能是：```+```,```-```,```*```,```/```, 还有括号

### 使用条件：
* 可自行选择编译运行环境（以下说明以Windows10环境为例）
* 须安装JDK并配置相关环境变量
* 你编写的Java程序须严格遵守以上约定

### 使用方法：

* 下载后，请将 ```exprchecker.exe``` 与将要执行的类 ```ExpressionGenerator.class``` 放置在统一目录，如：

```
ProjectDir
├── ExpressionGenerator.class
├── exprchecker.exe
├── ...
└── Other.class
```

* 双击 ```exprchecker.exe``` 将自动执行你的类 ```ExpressionGenerator.class```，按照以上约定的参数，执行6组测试，输出信息如下：

```
Executing command: "java ExpressionGenerator 10 1 10.txt" ... DONE with status: 0
Checking your result in 10.txt ... DONE
Generating report into report_10.txt ... DONE

Executing command: "java ExpressionGenerator 30 1 30.txt" ... DONE with status: 0
Checking your result in 30.txt ... DONE
Generating report into report_30.txt ... DONE

Executing command: "java ExpressionGenerator 100 1 100.txt" ... DONE with status: 0
Checking your result in 100.txt ... DONE
Generating report into report_100.txt ... DONE

Executing command: "java ExpressionGenerator 1000 3 1000.txt" ... DONE with status: 0
Checking your result in 1000.txt ... DONE
Generating report into report_1000.txt ... DONE

Executing command: "java ExpressionGenerator 10000 5 10000.txt" ... DONE with status: 0
Checking your result in 10000.txt ... DONE
Generating report into report_10000.txt ... DONE

Executing command: "java ExpressionGenerator 20000 7 20000.txt" ... DONE with status: 0
Checking your result in 20000.txt ... DONE
Generating report into report_20000.txt ... DONE

Press [Enter] to exit ...
```

#### 查看 ```report_10.txt```:

```
===============================
total expressions: 10
duplicated expressions: 0
===============================
Generation cost 125 ms.
```

- 生成10道题目
- 没有发现重复的题目
- 生成题目耗时125毫秒

#### 查看 ```report_30.txt```:

```
[2 + 3] @ line 12 is repeated with:
   -> [2 + 3] @ line 21

[7 / 8] @ line 10 is repeated with:
   -> [7 / 8] @ line 19

[9 * 1] @ line 2 is repeated with:
   -> [9 * 1] @ line 29

[4 * 5] @ line 4 is repeated with:
   -> [4 * 5] @ line 13

[1 + 2] @ line 7 is repeated with:
   -> [1 + 2] @ line 25

[5 + 6] @ line 9 is repeated with:
   -> [5 + 6] @ line 18

===============================
total expressions: 30
duplicated expressions: 6
===============================
Generation cost 133 ms.
```

- 生成30道题目
- 发现6道题目重复（未成功去重哦！）
- 生成题目耗时133毫秒
- 重复题目的详细信息在文件前面部分说明
