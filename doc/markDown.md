# 流程图



```mermaid
st=>start: 开始框
op=>operation: 处理框
cond=>condition: 判断框(是或否?)
sub1=>subroutine: 子流程
io=>inputoutput: 输入输出框
e=>end: 结束框
st->op->cond
cond(yes)->io->e
cond(no)->sub1(right)->op
```


graph TD;
    A-->B;
    A-->C;
    B-->D;
    C-->D;


```mermaid
graph TD
   A --> B
```
