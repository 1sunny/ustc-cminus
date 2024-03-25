; ModuleID = 'cminus'
source_filename = "/my/cminus-compiler/tests/section1/functional_test/65_word_count.sy"

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

define i32 @wc(i32* %arg0, i32 %arg1) {
label_entry:
  %op2 = alloca i32*
  store i32* %arg0, i32** %op2
  %op3 = alloca i32
  store i32 %arg1, i32* %op3
  %op4 = alloca i32
  %op5 = alloca i32
  %op6 = alloca i32
  store i32 0, i32* %op5
  store i32 0, i32* %op4
  store i32 0, i32* %op6
  br label %label_condBB_0
label_condBB_0:                                                ; preds = %label_entry, %label_exitBB_1
  %op7 = load i32, i32* %op5
  %op8 = load i32, i32* %op3
  %op9 = icmp slt i32 %op7, %op8
  br i1 %op9, label %label_loopBB_0, label %label_successorBB_0
label_loopBB_0:                                                ; preds = %label_condBB_0
  %op10 = load i32*, i32** %op2
  %op11 = load i32, i32* %op5
  %op12 = getelementptr i32, i32* %op10, i32 %op11
  %op13 = load i32, i32* %op12
  %op14 = icmp ne i32 %op13, 32
  br i1 %op14, label %label_trueBB_1, label %label_falseBB_1
label_successorBB_0:                                                ; preds = %label_condBB_0
  %op15 = load i32, i32* %op6
  ret i32 %op15
label_trueBB_1:                                                ; preds = %label_loopBB_0
  %op16 = load i32, i32* %op4
  %op17 = icmp eq i32 %op16, 0
  br i1 %op17, label %label_trueBB_2, label %label_exitBB_2
label_falseBB_1:                                                ; preds = %label_loopBB_0
  store i32 0, i32* %op4
  br label %label_exitBB_1
label_exitBB_1:                                                ; preds = %label_exitBB_2, %label_falseBB_1
  %op18 = load i32, i32* %op5
  %op19 = add i32 %op18, 1
  store i32 %op19, i32* %op5
  br label %label_condBB_0
label_trueBB_2:                                                ; preds = %label_trueBB_1
  %op20 = load i32, i32* %op6
  %op21 = add i32 %op20, 1
  store i32 %op21, i32* %op6
  store i32 1, i32* %op4
  br label %label_exitBB_2
label_exitBB_2:                                                ; preds = %label_trueBB_1, %label_trueBB_2
  br label %label_exitBB_1
}
define i32 @main() {
label_entry:
  %op0 = alloca [500 x i32]
  %op1 = alloca i32
  %op2 = alloca i32
  store i32 0, i32* %op2
  store i32 0, i32* %op1
  br label %label_condBB_3
label_condBB_3:                                                ; preds = %label_entry, %label_loopBB_3
  %op3 = load i32, i32* %op1
  %op4 = icmp ne i32 %op3, 10
  br i1 %op4, label %label_loopBB_3, label %label_successorBB_3
label_loopBB_3:                                                ; preds = %label_condBB_3
  %op5 = call i32 @getch()
  store i32 %op5, i32* %op1
  %op6 = load i32, i32* %op1
  %op7 = load i32, i32* %op2
  %op8 = getelementptr [500 x i32], [500 x i32]* %op0, i32 0, i32 %op7
  store i32 %op6, i32* %op8
  %op9 = load i32, i32* %op2
  %op10 = add i32 %op9, 1
  store i32 %op10, i32* %op2
  br label %label_condBB_3
label_successorBB_3:                                                ; preds = %label_condBB_3
  %op11 = getelementptr [500 x i32], [500 x i32]* %op0, i32 0, i32 0
  %op12 = load i32, i32* %op2
  %op13 = call i32 @wc(i32* %op11, i32 %op12)
  store i32 %op13, i32* %op1
  %op14 = load i32, i32* %op1
  call void @putint(i32 %op14)
  ret i32 0
}
