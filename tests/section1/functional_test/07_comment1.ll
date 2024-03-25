; ModuleID = 'cminus'
source_filename = "/my/cminus-compiler/tests/section1/functional_test/07_comment1.sy"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

declare i32 @getint()

declare i32 @getch()

declare float @getfloat()

declare i32 @getarray(i32*)

declare i32 @getfarray(float*)

declare void @putint(i32)

declare void @putch(i32)

declare void @putarray(i32, i32*)

declare void @putfloat(float)

declare void @putfarray(i32, float*)

define i32 @main() {
label_entry:
  ret i32 0
}
