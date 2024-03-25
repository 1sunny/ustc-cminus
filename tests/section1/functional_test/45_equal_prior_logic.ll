; ModuleID = 'cminus'
source_filename = "/my/cminus-compiler/tests/section1/functional_test/45_equal_prior_logic.sy"

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
  %op0 = alloca i32
  %op1 = alloca i32
  %op2 = alloca i32
  %op3 = alloca i32
  store i32 10, i32* %op0
  store i32 6, i32* %op1
  store i32 4, i32* %op2
  store i32 5, i32* %op3
  %op4 = alloca i32
  %op5 = load i32, i32* %op1
  %op6 = load i32, i32* %op2
  %op7 = add i32 %op5, %op6
  %op8 = load i32, i32* %op0
  %op9 = icmp eq i32 %op7, %op8
  br i1 %op9, label %label_trueBB_1, label %label_exitBB_0
label_trueBB_0:                                                ; preds = %label_trueBB_1
  %op10 = load i32, i32* %op1
  %op11 = load i32, i32* %op2
  %op12 = load i32, i32* %op3
  %op13 = sdiv i32 %op11, %op12
  %op14 = mul i32 %op13, 2
  %op15 = add i32 %op10, %op14
  store i32 %op15, i32* %op4
  %op16 = load i32, i32* %op4
  call void @putint(i32 %op16)
  br label %label_exitBB_0
label_exitBB_0:                                                ; preds = %label_entry, %label_trueBB_1, %label_trueBB_0
  %op17 = load i32, i32* %op2
  %op18 = icmp slt i32 %op17, 0
  br i1 %op18, label %label_trueBB_2, label %label_falseBB_3
label_trueBB_1:                                                ; preds = %label_entry
  %op19 = load i32, i32* %op3
  %op20 = load i32, i32* %op0
  %op21 = sdiv i32 %op20, 2
  %op22 = icmp ne i32 %op19, %op21
  br i1 %op22, label %label_trueBB_0, label %label_exitBB_0
label_trueBB_2:                                                ; preds = %label_exitBB_0, %label_trueBB_4
  store i32 1, i32* %op4
  %op23 = load i32, i32* %op4
  call void @putint(i32 %op23)
  br label %label_exitBB_2
label_exitBB_2:                                                ; preds = %label_falseBB_3, %label_trueBB_4, %label_trueBB_2
  ret i32 0
label_falseBB_3:                                                ; preds = %label_exitBB_0
  %op24 = load i32, i32* %op0
  %op25 = load i32, i32* %op2
  %op26 = sub i32 %op24, %op25
  %op27 = load i32, i32* %op1
  %op28 = icmp eq i32 %op26, %op27
  br i1 %op28, label %label_trueBB_4, label %label_exitBB_2
label_trueBB_4:                                                ; preds = %label_falseBB_3
  %op29 = load i32, i32* %op0
  %op30 = load i32, i32* %op3
  %op31 = mul i32 %op30, 2
  %op32 = icmp ne i32 %op29, %op31
  br i1 %op32, label %label_trueBB_2, label %label_exitBB_2
}
