; ModuleID = 'cminus'
source_filename = "/my/cminus-compiler/tests/section1/functional_test/98_many_local_var.sy"

@n = global i32 zeroinitializer
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
  %op4 = alloca i32
  %op5 = alloca i32
  %op6 = alloca i32
  %op7 = alloca i32
  %op8 = alloca i32
  %op9 = alloca i32
  %op10 = alloca i32
  %op11 = alloca i32
  %op12 = alloca i32
  %op13 = alloca i32
  %op14 = alloca i32
  %op15 = alloca i32
  %op16 = alloca i32
  %op17 = alloca i32
  %op18 = alloca i32
  %op19 = alloca i32
  %op20 = alloca i32
  %op21 = alloca i32
  %op22 = alloca i32
  %op23 = alloca i32
  %op24 = alloca i32
  %op25 = alloca i32
  %op26 = alloca i32
  %op27 = alloca i32
  %op28 = alloca i32
  %op29 = alloca i32
  %op30 = alloca i32
  %op31 = call i32 @getint()
  store i32 %op31, i32* %op30
  br label %label_condBB_0
label_condBB_0:                                                ; preds = %label_entry, %label_loopBB_0
  %op32 = load i32, i32* %op30
  %op33 = icmp eq i32 %op32, 5
  br i1 %op33, label %label_loopBB_0, label %label_successorBB_0
label_loopBB_0:                                                ; preds = %label_condBB_0
  %op34 = load i32, i32* %op30
  %op35 = add i32 %op34, 1
  store i32 %op35, i32* %op30
  br label %label_condBB_0
label_successorBB_0:                                                ; preds = %label_condBB_0
  store i32 0, i32* %op0
  %op36 = load i32, i32* %op0
  %op37 = add i32 %op36, 1
  store i32 %op37, i32* %op1
  %op38 = load i32, i32* %op1
  %op39 = add i32 %op38, 1
  store i32 %op39, i32* %op2
  %op40 = load i32, i32* %op2
  %op41 = add i32 %op40, 1
  store i32 %op41, i32* %op3
  %op42 = load i32, i32* %op3
  %op43 = add i32 %op42, 1
  store i32 %op43, i32* %op4
  %op44 = load i32, i32* %op4
  %op45 = add i32 %op44, 1
  store i32 %op45, i32* %op5
  %op46 = load i32, i32* %op5
  %op47 = add i32 %op46, 1
  store i32 %op47, i32* %op6
  %op48 = load i32, i32* %op6
  %op49 = add i32 %op48, 1
  store i32 %op49, i32* %op7
  %op50 = load i32, i32* %op7
  %op51 = add i32 %op50, 1
  store i32 %op51, i32* %op8
  %op52 = load i32, i32* %op8
  %op53 = add i32 %op52, 1
  store i32 %op53, i32* %op9
  %op54 = load i32, i32* %op9
  %op55 = add i32 %op54, 1
  store i32 %op55, i32* %op10
  %op56 = load i32, i32* %op10
  %op57 = add i32 %op56, 1
  store i32 %op57, i32* %op11
  %op58 = load i32, i32* %op11
  %op59 = add i32 %op58, 1
  store i32 %op59, i32* %op12
  %op60 = load i32, i32* %op12
  %op61 = add i32 %op60, 1
  store i32 %op61, i32* %op13
  %op62 = load i32, i32* %op13
  %op63 = add i32 %op62, 1
  store i32 %op63, i32* %op14
  %op64 = load i32, i32* %op14
  %op65 = add i32 %op64, 1
  store i32 %op65, i32* %op15
  %op66 = load i32, i32* %op15
  %op67 = add i32 %op66, 1
  store i32 %op67, i32* %op16
  %op68 = load i32, i32* %op16
  %op69 = add i32 %op68, 1
  store i32 %op69, i32* %op17
  %op70 = load i32, i32* %op17
  %op71 = add i32 %op70, 1
  store i32 %op71, i32* %op18
  %op72 = load i32, i32* %op18
  %op73 = add i32 %op72, 1
  store i32 %op73, i32* %op19
  %op74 = load i32, i32* %op19
  %op75 = add i32 %op74, 1
  store i32 %op75, i32* %op20
  %op76 = load i32, i32* %op20
  %op77 = add i32 %op76, 1
  store i32 %op77, i32* %op21
  %op78 = load i32, i32* %op21
  %op79 = add i32 %op78, 1
  store i32 %op79, i32* %op22
  %op80 = load i32, i32* %op22
  %op81 = add i32 %op80, 1
  store i32 %op81, i32* %op23
  %op82 = load i32, i32* %op23
  %op83 = add i32 %op82, 1
  store i32 %op83, i32* %op24
  %op84 = load i32, i32* %op24
  %op85 = add i32 %op84, 1
  store i32 %op85, i32* %op25
  %op86 = load i32, i32* %op25
  %op87 = add i32 %op86, 1
  store i32 %op87, i32* %op26
  %op88 = load i32, i32* %op26
  %op89 = add i32 %op88, 1
  store i32 %op89, i32* %op27
  %op90 = load i32, i32* %op27
  %op91 = add i32 %op90, 1
  store i32 %op91, i32* %op28
  %op92 = load i32, i32* %op28
  %op93 = add i32 %op92, 1
  store i32 %op93, i32* %op29
  %op94 = alloca i32
  %op95 = load i32, i32* %op0
  call void @putint(i32 %op95)
  %op96 = load i32, i32* %op1
  call void @putint(i32 %op96)
  %op97 = load i32, i32* %op2
  call void @putint(i32 %op97)
  %op98 = load i32, i32* %op3
  call void @putint(i32 %op98)
  %op99 = load i32, i32* %op4
  call void @putint(i32 %op99)
  %op100 = load i32, i32* %op5
  call void @putint(i32 %op100)
  %op101 = load i32, i32* %op6
  call void @putint(i32 %op101)
  %op102 = load i32, i32* %op7
  call void @putint(i32 %op102)
  %op103 = load i32, i32* %op8
  call void @putint(i32 %op103)
  %op104 = load i32, i32* %op9
  call void @putint(i32 %op104)
  %op105 = load i32, i32* %op10
  call void @putint(i32 %op105)
  %op106 = load i32, i32* %op11
  call void @putint(i32 %op106)
  %op107 = load i32, i32* %op12
  call void @putint(i32 %op107)
  %op108 = load i32, i32* %op13
  call void @putint(i32 %op108)
  %op109 = load i32, i32* %op14
  call void @putint(i32 %op109)
  %op110 = load i32, i32* %op15
  call void @putint(i32 %op110)
  %op111 = load i32, i32* %op16
  call void @putint(i32 %op111)
  %op112 = load i32, i32* %op17
  call void @putint(i32 %op112)
  %op113 = load i32, i32* %op18
  call void @putint(i32 %op113)
  %op114 = load i32, i32* %op19
  call void @putint(i32 %op114)
  %op115 = load i32, i32* %op20
  call void @putint(i32 %op115)
  %op116 = load i32, i32* %op21
  call void @putint(i32 %op116)
  %op117 = load i32, i32* %op22
  call void @putint(i32 %op117)
  %op118 = load i32, i32* %op23
  call void @putint(i32 %op118)
  %op119 = load i32, i32* %op24
  call void @putint(i32 %op119)
  %op120 = load i32, i32* %op25
  call void @putint(i32 %op120)
  %op121 = load i32, i32* %op26
  call void @putint(i32 %op121)
  %op122 = load i32, i32* %op27
  call void @putint(i32 %op122)
  %op123 = load i32, i32* %op28
  call void @putint(i32 %op123)
  %op124 = load i32, i32* %op29
  call void @putint(i32 %op124)
  %op125 = alloca i32
  store i32 10, i32* %op125
  %op126 = load i32, i32* %op125
  call void @putch(i32 %op126)
  %op127 = load i32, i32* %op30
  call void @putint(i32 %op127)
  %op128 = load i32, i32* %op125
  call void @putch(i32 %op128)
  %op129 = load i32, i32* %op25
  ret i32 %op129
}
