# Feature Requests
Want a new feature in Mudlet? Let us know about it! We also have a long-running feature requests thread on [our forums](http://forums.mudlet.org/viewtopic.php?f=5&t=92).

# Bug Reports
We're sorry you've encountered an issue with Mudlet and appreciate that you're taking the time to report it to us. 

To help us find & fix the problem quickest, please include:
* as much information possible about the issue
* your Mudlet version as it says in the ``About`` window
* specific instructions on how to reproduce the issue if you know how to

# Coding guidelines

If you're a first-timer, you're excluded, we'll go easy on you :wink:

## Use QLatin1String over QStringLiteral if the function takes it

Some methods in the Qt API have overloads for either taking a QString, or a QLatin1String object.
This is because Latin1 is simpler to parse than UTF-16, and therefore the QLatin1String version can
be faster, and use less memory. For example, QString::startsWith() has the following declarations:

```
bool	startsWith(const QString &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const
bool	startsWith(const QStringRef &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const
bool	startsWith(QLatin1String s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const
bool	startsWith(QChar c, Qt::CaseSensitivity cs = Qt::CaseSensitive) const
```

Notice how the 3rd variant accepts a `QLatin1String`? That means you can use it. `==` and `+=` also
accept it, so here are some examples:

```cpp
bool same = (str == QLatin1String("Hello"));
str.startsWith(QLatin1String("Hello"));
str += QLatin1String("World");
```

Examples above are more efficient than:

```cpp
bool same = (str == QStringLiteral("Hello"));
str.startsWith(QStringLiteral("Hello"));
str += QStringLiteral("World");
```

And even more than:

```cpp
bool same = (str == "Hello");
str.startsWith("Hello");
str += "World";
```

Not all functions are going to accept `QLatin1String` - `QIcon` constructors for example are the following:

```
QIcon()
QIcon(const QPixmap &pixmap)
QIcon(const QIcon &other)
QIcon(QIcon &&other)
QIcon(const QString &fileName)
QIcon(QIconEngine *engine)
```

No `QLatin1String` - mentioned - so use `QStringLiteral` instead, which creates us a `QString()` at compile-time so at least creating the object is faster.

([source](http://blog.qt.io/blog/2014/06/13/qt-weekly-13-qstringliteral/),
 [additional reading](https://woboq.com/blog/qstringliteral.html))

## Do not use ``QStringLiteral("")``

Prefer ``QString()`` over ``QStringLiteral("")`` for  for empty strings - the default constructor 
for QString is cheaper in terms of both instructions and memory.

([source](http://blog.qt.io/blog/2014/06/13/qt-weekly-13-qstringliteral/))

## Avoid duplicated QStringLiterals

Avoid having multiple QStringLiterals with the same content. For plain literals and QLatin1String, compilers
try to consolidate identical literals so that they are not duplicated. For QStringLiteral, identical strings
cannot be merged.

([source](http://blog.qt.io/blog/2014/06/13/qt-weekly-13-qstringliteral/))

## Use .arg(arg1, arg2) instead of .arg(arg1).arg(arg2)

If you just need to use a plain .arg() call, use multiple arguments within it and not as separate .arg() calls as that'll avoid creating temporary QString objects. If your 2nd and 3rd arguments happen to be numbers, be sure to wrap them in  `QString::number(yourNumber)`.

([source](https://meetingcpp.com/tl_files/mcpp/2015/talks/Marc-Mutz-MC++15-Effective-Qt.pdf))

# Internationalization do's and don'ts

Do:
* enable strings visible in the Mudlet GUI to be translateable
* minimise use of HTML styling tags in strings to be translated
* enable users to use language-specific Mudlet object names (triggers, aliases, labels, etc)

Don't:
* translate the Mudlet API: functions, events, error messages or constants (e.g. `main` console)
* use numbers in the API - English words are preferred instead
* try to assemble a sentence on the fly - English grammar does not translate into other languages. Present the full sentence to translators instead
* assume English-centric plural forms, other languages do not necessarily have the simple add an "s"/"es" for more/less then the singular case.
* assume universal quote and number punctuation formats. There are languages that use « and » instead of " for "quoting" words or phrases. Qt can provide Locale specific displays of numbers/dates/times.

# Git commit guidelines for core team

## Merging Pull Requests (PRs)

The preferred order of [merging PRs](https://help.github.com/articles/about-pull-request-merges/) is: prefer rebase, else squash if you'd like to clean up the history before merging. Avoid doing a merge commit.
