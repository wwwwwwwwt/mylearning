# mysql有时会选错索引

## 解决方法：

如果发现运行时间比自己想的多很久，那么打开慢查询日志,设置阈值为0 set query_long_time = 0 打开慢查询日志 show variable like '% slow_query_log%'; 使用force index（某个索引）强制使用某个索引，进行对比