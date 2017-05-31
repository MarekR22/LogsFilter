# LogsFilter
Application helps analyze logs.
It is also a good example how [QAbstractTableModel](http://doc.qt.io/qt-5/qabstracttablemodel.html)

# Use
1. Create configuration file, which contains
* `startEntryRegExp`    - [regular expression](http://doc.qt.io/qt-5/qregularexpression.html#details) for detecting line containing timestamp
* `timeFormat`          - [format](http://doc.qt.io/qt-5/qdatetime.html#toString) use to convert string to date-time
* `timeCaptureIndex`    - capture index of regular expression `startEntryRegExp` which will provide date-time
* `tagCaptureIndex`     - capture index of tag column
* `logTextCaptureIndex` - capture index of content of logs entry
```json
{
    "startEntryRegExp"    : "(\\d+-\\d+-\\d+ \\d+:\\d{2}:\\d{2}.\\d{3}) \\[(\\w+)\\]\\s*(.*)",
    "timeFormat"          : "yyyy-MM-dd HH:mm:ss.zzz",
    "timeCaptureIndex"    : 1,
    "tagCaptureIndex"     : 2,
    "logTextCaptureIndex" : 3
}
```
2. Load it to application
3. Load logs which suppose to be analyzed
4. Select time zone of logs to show time in local time
5. Provide filter pattern: literal or regular expression
