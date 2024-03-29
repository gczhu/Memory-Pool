实现原理：
在该程序的内存池实现中，每次申请一大块内存，然后根据需要分配内存的大小，将一大块内存切成两部分；
一部分提供给申请者，另一部分则留着以后来用，将其插入一个双向链表中；
当下一次内存请求来临时，就首先判断链表中的空闲内存的大小是否满足要求；
如果满足要求，就将该空闲内存切成两部分，一部分提供给外界，另一部分仍留在链表中；
另外，为了便于维护双向链表，每次向链表中插入或者删除节点时，都需要对链表进行更新，使得更大的内存块靠近头节点；

测试方法：
为了便于测试，code文件夹中不仅向您提供了Makefile文件，还提供了一个run.sh脚本
您可以运行Makefile来调用test.cpp进行一次测试，也可以直接运行run.sh脚本对不同size的运行情况进行测试

注意事项：
我在测试过程中发现，运行结果受到电脑性能的影响较大
在我的电脑上，使用内存池实现的allocator比系统的allocator更快，运行结果可参考"test_result"文件夹中的屏幕截图
然而，在另一位同学的电脑上运行后发现内存池实现的allocator更慢，初步推测可能是电脑性能不同导致的
另外，使用test.cpp文件测试过程中可能会受到其他程序的干扰，建议关闭其他程序，多次测试，以提高测试结果的准确性
通过大量测试后再进行统计，可以发现使用内存池实现的allocator确实是更快的