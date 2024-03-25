; ModuleID = 'cminus'
source_filename = "/my/cminus-compiler/tests/section1/functional_test/14_or.sy"

@a = global i32 zeroinitializer
@b = global i32 zeroinitializer
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
  %op0 = call i32 @getint()
  store i32 %op0, i32* @a
  %op1 = call i32 @getint()
  store i32 %op1, i32* @b
  %op2 = load i32, i32* @a
  %op3 = icmp ne i32 %op2, 0
  br i1 %op3, label %label_trueBB_0, label %label_falseBB_1
label_trueBB_0:                                                ; preds = %label_entry, %label_falseBB_1
  ret i32 1
label_falseBB_0:                                                ; preds = %label_falseBB_1
  ret i32 0
label_falseBB_1:                                                ; preds = %label_entry
  %op4 = load i32, i32* @b
  %op5 = icmp ne i32 %op4, 0
  br i1 %op5, label %label_trueBB_0, label %label_falseBB_0
}
