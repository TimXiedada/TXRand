# TXRand文档——数组相关操作及其他函数

本文介绍数组相关操作函数及其他函数，位于arrop.c。

## arrop.c

### `sample`函数

```c
void* sample(const void* pop, size_t size, size_t count, size_t k, void* dest);
```

从数组中选择唯一元素的 *`k`* 长度数组，放入*`dest`*指向的数组。 用于无重复的随机抽样。

#### 参数列表：

| 参数 | 值 |
|  :----  |  ----  |
|*`pop`*|总体数组的地址|
|*`size`*|每个元素的长度|
|*`count`*|总体数组的元素个数|
|*`k`*|抽样数组的元素个数|
|*`dest`*|抽样数组的地址|

#### 返回值

若成功，返回抽样数组的地址（*dest*），失败时返回`NULL`。

---

### `shuffle`函数

```c
void shuffle(void* x, size_t size, size_t count);
```

将数组 *`x`* 随机打乱位置。

#### 参数列表：

| 参数 | 值 |
|  :----  |  ----  |
|*`x`*|数组的地址|
|*`size`*|每个元素的长度|
|*`count`*|数组的元素个数|

#### `choice`函数

```c

```