# Apache Thrift Language Support #

Last Modified: 2017-10-04<br>
Version: 0.10.0+

Thrift supports many programming languages and has an impressive test suite that exercises most of the languages, protocols, and transports that represents a matrix of thousands of possible combinations.  Each language typically has a minimum required version as well as support libraries - some mandatory and some optional.  All of this information is provided below to help you assess whether you can use Apache Thrift with your project.  Obviously this is a complex matrix to maintain and may not be correct in all cases - if you spot an error please inform the developers using the mailing list.

Apache Thrift has a choice of two build systems.  The `autoconf` build system is the most complete build and is used to build all supported languages.  The `cmake` build system has been designated by the project to replace `autoconf` however this transition will take quite some time to complete.  

The Language/Library levels indicate the minimum and maximum versions that are used in the continuous integration environments (Appveyor, Travis) for Apache Thrift.  Note that while a language may contain support for protocols, transports, and servers, the extent to which each is tested as part of the overall build process varies.  The definitive integration test for the project is called the "cross" test which executes a test matrix with clients and servers communicating across languages.

<style>
tr:hover { background-color: #99ddff; } 
table.matrix { font-size: 75%; border-spacing: 0px; }
</style>

<table class="matrix">
<thead>
<tr>
<th rowspan=2>Language</th>
<th colspan=2 align=center>Build Systems</th>
<th colspan=2 align=center>Lang/Lib Levels</th>
<th colspan=6 align=center>Low-Level Transports</th>
<th colspan=3 align=center>Transport Wrappers</th>
<th colspan=4 align=center>Protocols</th>
<th colspan=5 align=center>Servers</th>
<th rowspan=2>Open Issues</th>
</tr>
<tr>
<!-- Build Systems ---------><th>autoconf</th><th>cmake</th>
<!-- Lang/Lib Levels -------><th>Min</th><th>Max</th>
<!-- Low-Level Transports --><th>[Domain](https://en.wikipedia.org/wiki/Unix_domain_socket)</th><th>&nbsp;File&nbsp;</th><th>Memory</th><th>&nbsp;Pipe&nbsp;</th><th>Socket</th><th>&nbsp;TLS&nbsp;</th>
<!-- Transport Wrappers ----><th>Framed</th><th>&nbsp;http&nbsp;</th><th>&nbsp;zlib&nbsp;</th>
<!-- Protocols -------------><th>[Binary](doc/specs/thrift-binary-protocol.md)</th><th>[Compact](doc/specs/thrift-compact-protocol.md)</th><th>&nbsp;JSON&nbsp;</th><th>Multiplex</th>
<!-- Servers ---------------><th>Forking</th><th>Nonblocking</th><th>Simple</th><th>Threaded</th><th>ThreadPool</th>
</tr>
</thead>
<tbody>
<tr align=center>
<td align=left>[ActionScript](lib/as3/README.md)</td>
<!-- Build Systems ---------><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Language Levels -------><td colspan=2>ActionScript 3</td>
<!-- Low-Level Transports --><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Transport Wrappers ----><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Protocols -------------><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Servers ---------------><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<td align=left>[ActionScript](https://issues.apache.org/jira/browse/THRIFT/component/12313722)</td>
</tr>
<tr align=center>
<td align=left>[C (glib)](lib/c_glib/README.md)</td>
<!-- Build Systems ---------><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Language Levels -------><td>2.40.2</td><td>2.54.0</td>
<!-- Low-Level Transports --><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Transport Wrappers ----><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Protocols -------------><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Servers ---------------><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<td align=left>[C (glib)](https://issues.apache.org/jira/browse/THRIFT/component/12313854)</td>
</tr>
<tr align=center>
<td align=left>[C++](lib/cpp/README.md)</td>
<!-- Build Systems ---------><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Language Levels -------><td colspan=2>C++98, gcc </td>
<!-- Low-Level Transports --><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Transport Wrappers ----><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Protocols -------------><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Servers ---------------><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<td align=left>[C++](https://issues.apache.org/jira/browse/THRIFT/component/12312313)</td>
</tr>
<tr align=center>
<td align=left>[C# (.NET)](lib/csharp/README.md)</td>
<!-- Build Systems ---------><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Language Levels -------><td>.NET&nbsp;3.5 / mono&nbsp;3.2.8.0</td><td>.NET&nbsp;4.6.1 / mono&nbsp;4.6.2.7</td>
<!-- Low-Level Transports --><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Transport Wrappers ----><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Protocols -------------><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Servers ---------------><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<td align=left>[C# (.NET)](https://issues.apache.org/jira/browse/THRIFT/component/12312362)</td>
</tr>
<tr align=center>
<td align=left>[Cocoa](lib/cocoa/README.md)</td>
<!-- Build Systems ---------><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Language Levels -------><td colspan=2>unknown</td>
<!-- Low-Level Transports --><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Transport Wrappers ----><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Protocols -------------><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Servers ---------------><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<td align=left>[Cocoa](https://issues.apache.org/jira/browse/THRIFT/component/12312398)</td>
</tr>
<tr align=center>
<td align=left>[D](lib/d/README.md)</td>
<!-- Build Systems ---------><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Language Levels -------><td>2.070.2</td><td>2.076.0</td>
<!-- Low-Level Transports --><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Transport Wrappers ----><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Protocols -------------><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Servers ---------------><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<td align=left>[D](https://issues.apache.org/jira/browse/THRIFT/component/12317904)</td>
</tr>
<tr align=center>
<td align=left>[Dart](lib/dart/README.md)</td>
<!-- Build Systems ---------><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Language Levels -------><td>1.20.1</td><td>1.24.2</td>
<!-- Low-Level Transports --><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Transport Wrappers ----><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Protocols -------------><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Servers ---------------><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<td align=left>[Dart](https://issues.apache.org/jira/browse/THRIFT/component/12328006)</td>
</tr>
<tr align=center>
<td align=left>[Delphi](lib/delphi/README.md)</td>
<!-- Build Systems ---------><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Language Levels -------><td>2010</td><td>unknown</td>
<!-- Low-Level Transports --><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Transport Wrappers ----><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Protocols -------------><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Servers ---------------><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<td align=left>[Delphi](https://issues.apache.org/jira/browse/THRIFT/component/12316601)</td>
</tr>
<tr align=center>
<td align=left>[.NET Core](lib/netcore/README.md)</td>
<!-- Build Systems ---------><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Language Levels -------><td colspan=2>1.0.0-preview2-1-003177</td>
<!-- Low-Level Transports --><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Transport Wrappers ----><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Protocols -------------><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Servers ---------------><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<td align=left>[.NET Core](https://issues.apache.org/jira/browse/THRIFT/component/12331176)</td>
</tr>
<tr align=center>
<td align=left>[Erlang](lib/erl/README.md)</td>
<!-- Build Systems ---------><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Language Levels -------><td>R16B03</td><td>20.0.4</td>
<!-- Low-Level Transports --><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Transport Wrappers ----><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Protocols -------------><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Servers ---------------><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<td align=left>[Erlang](https://issues.apache.org/jira/browse/THRIFT/component/12312390)</td>
</tr>
<tr align=center>
<td align=left>[Go](lib/go/README.md)</td>
<!-- Build Systems ---------><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Language Levels -------><td>1.2.1</td><td>1.8.3</td>
<!-- Low-Level Transports --><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Transport Wrappers ----><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Protocols -------------><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Servers ---------------><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<td align=left>[Go](https://issues.apache.org/jira/browse/THRIFT/component/12314307)</td>
</tr>
<tr align=center>
<td align=left>[Haskell](lib/hs/README.md)</td>
<!-- Build Systems ---------><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Language Levels -------><td>7.6.3</td><td>8.0.2</td>
<!-- Low-Level Transports --><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Transport Wrappers ----><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Protocols -------------><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Servers ---------------><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<td align=left>[Haskell](https://issues.apache.org/jira/browse/THRIFT/component/12312704)</td>
</tr>
<tr align=center>
<td align=left>[Haxe](lib/haxe/README.md)</td>
<!-- Build Systems ---------><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Language Levels -------><td colspan=2>3.2.1</td>
<!-- Low-Level Transports --><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Transport Wrappers ----><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Protocols -------------><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Servers ---------------><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<td align=left>[Haxe](https://issues.apache.org/jira/browse/THRIFT/component/12324347)</td>
</tr>
<tr align=center>
<td align=left>[Java](lib/java/README.md)</td>
<!-- Build Systems ---------><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Language Levels -------><td>1.7.0_151</td><td>1.8.0_144</td>
<!-- Low-Level Transports --><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Transport Wrappers ----><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Protocols -------------><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Servers ---------------><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<td align=left>[Java](https://issues.apache.org/jira/browse/THRIFT/component/12312314)</td>
</tr>
<tr align=center>
<td align=left>[Java ME](lib/javame/README.md)</td>
<!-- Build Systems ---------><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Language Levels -------><td colspan=2>unknown</td>
<!-- Low-Level Transports --><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Transport Wrappers ----><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Protocols -------------><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Servers ---------------><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<td align=left>[Java ME](https://issues.apache.org/jira/browse/THRIFT/component/12313759)</td>
</tr>
<tr align=center>
<td align=left>[Javascript](lib/js/README.md)</td>
<!-- Build Systems ---------><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Language Levels -------><td>unknown</td>
<!-- Low-Level Transports --><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Transport Wrappers ----><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Protocols -------------><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Servers ---------------><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<td align=left>[Javascript](https://issues.apache.org/jira/browse/THRIFT/component/12313418)</td>
</tr>
<tr align=center>
<td align=left>[Lua](lib/lua/README.md)</td>
<!-- Build Systems ---------><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Language Levels -------><td>5.1.5</td><td>5.3.3</td>
<!-- Low-Level Transports --><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Transport Wrappers ----><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Protocols -------------><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Servers ---------------><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<td align=left>[Lua](https://issues.apache.org/jira/browse/THRIFT/component/12322659)</td>
</tr>
<tr align=center>
<td align=left>[node.js](lib/nodejs/README.md)</td>
<!-- Build Systems ---------><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Language Levels -------><td>4.2.6</td><td>6.11.2</td>
<!-- Low-Level Transports --><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Transport Wrappers ----><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Protocols -------------><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Servers ---------------><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<td align=left>[node.js](https://issues.apache.org/jira/browse/THRIFT/component/12314320)</td>
</tr>
<tr align=center>
<td align=left>[OCaml](lib/ocaml/README.md)</td>
<!-- Build Systems ---------><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Language Levels -------><td>4.02.3</td><td>4.04.0</td>
<!-- Low-Level Transports --><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Transport Wrappers ----><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Protocols -------------><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Servers ---------------><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<td align=left>[OCaml](https://issues.apache.org/jira/browse/THRIFT/component/12313660)</td>
</tr>
<tr align=center>
<td align=left>[Perl](lib/perl/README.md)</td>
<!-- Build Systems ---------><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Language Levels -------><td>5.18.2</td><td>5.26.0</td>
<!-- Low-Level Transports --><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Transport Wrappers ----><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Protocols -------------><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Servers ---------------><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<td align=left>[Perl](https://issues.apache.org/jira/browse/THRIFT/component/12312312)</td>
</tr>
<tr align=center>
<td align=left>[PHP](lib/php/README.md)</td>
<!-- Build Systems ---------><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Language Levels -------><td>5.5.9</td><td>7.1.8</td>
<!-- Low-Level Transports --><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Transport Wrappers ----><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Protocols -------------><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Servers ---------------><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<td align=left>[PHP](https://issues.apache.org/jira/browse/THRIFT/component/12312431)</td>
</tr>
<tr align=center>
<td align=left>[Python](lib/py/README.md)</td>
<!-- Build Systems ---------><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Language Levels -------><td>2.7.6, 3.4.3</td><td>2.7.14, 3.6.3</td>
<!-- Low-Level Transports --><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Transport Wrappers ----><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Protocols -------------><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Servers ---------------><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<td align=left>[Python](https://issues.apache.org/jira/browse/THRIFT/component/12312315)</td>
</tr>
<tr align=center>
<td align=left>[Ruby](lib/rb/README.md)</td>
<!-- Build Systems ---------><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Language Levels -------><td>1.9.3p484</td><td>2.3.3p222</td>
<!-- Low-Level Transports --><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Transport Wrappers ----><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Protocols -------------><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Servers ---------------><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<td align=left>[Ruby](https://issues.apache.org/jira/browse/THRIFT/component/12312316)</td>
</tr>
<tr align=center>
<td align=left>[Rust](lib/rs/README.md)</td>
<!-- Build Systems ---------><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Language Levels -------><td>1.15.1</td><td>1.18.0</td>
<!-- Low-Level Transports --><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Transport Wrappers ----><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Protocols -------------><td>![Yes](doc/images/cgrn.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td>
<!-- Servers ---------------><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<td align=left>[Rust](https://issues.apache.org/jira/browse/THRIFT/component/12331420)</td>
</tr>
<tr align=center>
<td align=left>[Smalltalk](lib/st/README.md)</td>
<!-- Build Systems ---------><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Language Levels -------><td colspan=2>unknown</td>
<!-- Low-Level Transports --><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Transport Wrappers ----><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Protocols -------------><td>![Yes](doc/images/cgrn.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<!-- Servers ---------------><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td><td>![No](doc/images/cred.png)</td>
<td align=left>[Smalltalk](https://issues.apache.org/jira/browse/THRIFT/component/12313861)</td>
</tr>
</tbody>
</tfoot>
<tr>
<th rowspan=2>Language</th>
<!-- Build Systems ---------><th>autoconf</th><th>cmake</th>
<!-- Lang/Lib Levels -------><th>Min</th><th>Max</th>
<!-- Low-Level Transports --><th>[Domain](https://en.wikipedia.org/wiki/Unix_domain_socket)</th></th><th>&nbsp;File&nbsp;</th><th>Memory</th><th>&nbsp;Pipe&nbsp;</th><th>Socket</th><th>&nbsp;TLS&nbsp;</th>
<!-- Transport Wrappers ----><th>Framed</th><th>&nbsp;http&nbsp;</th><th>&nbsp;zlib&nbsp;</th>
<!-- Protocols -------------><th>[Binary](doc/specs/thrift-binary-protocol.md)</th><th>[Compact](doc/specs/thrift-compact-protocol.md)</th><th>&nbsp;JSON&nbsp;</th><th>Multiplex</th>
<!-- Servers ---------------><th>Forking</th><th>Nonblocking</th><th>Simple</th><th>Threaded</th><th>ThreadPool</th>
<th rowspan=2>Language</th>
</tr>
<tr>
<th colspan=2 align=center>Build Systems</th>
<th colspan=2 align=center>Lang/Lib Levels</th>
<th colspan=6 align=center>Low-Level Transports</th>
<th colspan=3 align=center>Transport Wrappers</th>
<th colspan=4 align=center>Protocols</th>
<th colspan=5 align=center>Servers</th>
</tr>
</tfoot>
</table>
