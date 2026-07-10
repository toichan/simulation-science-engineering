# 拡散方程式用 gnuplot スクリプトファイル
# gnuplot を立ち上げてから
#   gnuplot> load "plot.gp"
# とすれば読み込めるし、gnuplot を実行するときの引数にもできる。

# ポストスクリプトファイルに書き出すには、次の２行のコメントを外す
#set terminal postscript enhanced color 30
#set output "diffusion.ps"

set grid
set xlabel "x"
set ylabel "u"

# 次の行でプロット。ちょっとややこしいが、
#   using a:b
# はデータのうち a 列目と b 列目をそれぞれ横、縦軸としてプロットする、の意。
# b のところが
#  ($2==0.1 ? $6 : 1/0)
# となっているのは、C 言語のように if-then-else で分岐している。２列目が 0.1
# (つまり時刻が 0.1) のときに６列目を縦軸に使い、そうでなければ 1/0 = NaN
# を使う、つまり何も出力しない、ということ。これで任意の時刻が抜き出せる。
# 普通に
#   using 4:6
# とすると全時刻の履歴が見れる。
plot "diffusion.dat" using 4:($2==0.1 ? $6 : 1/0) title "time=0.1" with line
#plot "diffusion.dat" using 4:6 notitle with line
