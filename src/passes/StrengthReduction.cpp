#include "StrengthReduction.hpp"

void StrengthReduction::run() {
  specInstReductStrict();
}

bool StrengthReduction::isNthPower(int x) {
  return ((x & (x - 1)) == 0);
}

void StrengthReduction::specInstReductStrict() {
  std::set<Instruction *> deleteInst;

  for (auto fun: module_->get_functions()) {
    for (auto bb: fun->get_basic_blocks()) {
      IRBuilder builder(bb, module_);
      auto &instructions = bb->get_instructions();
      for (auto inst_iter = instructions.begin(); inst_iter != instructions.end(); inst_iter++) {
        auto inst = *inst_iter;
        if (!inst->is_binary()) {
          continue;
        }
        Value *value0 = inst->get_operand(0);
        Value *value1 = inst->get_operand(1);

        auto casted = ConstantPropagation::cast_to_const_int(value1);
        if (casted) {
          // 将第二个操作数为2的n次方的取余操作进行重写
          // c = a % b -> c = a & (b - 1)
          if (inst->is_srem()) {
            int op2_val = casted->get_value();
            if (op2_val == 1) {
              // a % 1 = 0
              inst->replace_all_use_with(ConstantInt::get(0, module_));
              deleteInst.insert(inst);
            } else if (isNthPower(casted->get_value())) {
              int k = (int) (ceil(std::log2(op2_val))) % 32;
              // 不能直接用 a & (k-1), 因为a可能是负数, -5 % 4 = -1
              // 正数:a%(2^k)=a&(2^k−1)
              // 负数:a%(2^k)=(a+(2^k-1))&(2^k−1)
              // 通用:a%2^k=a−((a>>(32−k))+a)&(2^k−1)

              // 获取符号位
              auto asr = builder.create_asr(value0, ConstantInt::get(31, module_));
              instructions.pop_back();
              bb->add_instruction(inst_iter, asr);
              auto lsr = builder.create_lsr(asr, ConstantInt::get((32 - k), module_));
              instructions.pop_back();
              bb->add_instruction(inst_iter, lsr);
              auto add = builder.create_iadd(lsr, value0);
              instructions.pop_back();
              bb->add_instruction(inst_iter, add);
              auto and_ = builder.create_iand(add, ConstantInt::get(-op2_val, module_));
              instructions.pop_back();
              bb->add_instruction(inst_iter, and_);
              auto sub = builder.create_isub(value0, and_);
              instructions.pop_back();
              bb->add_instruction(inst_iter, sub);

              inst->replace_all_use_with(sub);
              deleteInst.insert(inst);
            }
          }
          // *0, *1, *2的n次方转化为左移
          if (inst->is_mul()) {
            int op2_val = casted->get_value();
            if (op2_val == 0) {
              inst->replace_all_use_with(ConstantInt::get(0, module_));
              deleteInst.insert(inst);
            } else if (op2_val == 1) {
              inst->replace_all_use_with(value0);
              deleteInst.insert(inst);
            } else if (op2_val > 0 && isNthPower(casted->get_value())) {
              int log = 0;
              while (!(op2_val & 1)) {
                op2_val >>= 1;
                log++;
              }
              auto res = builder.create_lsl(value0, ConstantInt::get(log, module_));
              instructions.pop_back();
              bb->add_instruction(inst_iter, res);
              inst->replace_all_use_with(res);
              deleteInst.insert(inst);
            }
          }
          // /0, /1, /2, /2的n次方
          if (inst->is_div()) {
            int op2_val = casted->get_value();
            if (op2_val == 0) {
              std::cerr << "divided by zero!" << std::endl;
              exit(-1);
            } else if (op2_val == 1) {
              inst->replace_all_use_with(value0);
              deleteInst.insert(inst);
            } else if (op2_val == 2) {
              // a / 2 -> (a - (a >> 31)) >> 1
              // (a >> 1) + (a & 1): 对于负数,如果a是奇数,a / 2需要向上取整,而a >> 1会向下取整,因此加上(a & 1)
              auto sign = builder.create_asr(value0, ConstantInt::get(31, module_));
              instructions.pop_back();
              bb->add_instruction(inst_iter, sign);
              auto sub = builder.create_isub(value0, sign);
              instructions.pop_back();
              bb->add_instruction(inst_iter, sub);
              auto asr = builder.create_asr(value0, ConstantInt::get(1, module_));
              instructions.pop_back();
              bb->add_instruction(inst_iter, asr);
              inst->replace_all_use_with(asr);
              deleteInst.insert(inst);
            } else if (isNthPower(casted->get_value())) {
              // (x + ((x >>(算术) 31) >> (32 - k)) ) >>(算术) k
              // (x + ((x >>(算术) 31) & ((1 << k) + ~0)) ) >> k;
              // https://stackoverflow.com/questions/5061093/dividing-by-power-of-2-using-bit-shifting
              int k = (int) (ceil(std::log2(op2_val))) % 32;
              auto sign = builder.create_asr(value0, ConstantInt::get(31, module_));//符号位
              instructions.pop_back();
              bb->add_instruction(inst_iter, sign);
              auto lsr = builder.create_lsr(sign, ConstantInt::get(32 - k, module_));
              instructions.pop_back();
              bb->add_instruction(inst_iter, lsr);
              auto add = builder.create_iadd(value0, lsr);
              instructions.pop_back();
              bb->add_instruction(inst_iter, add);
              auto asr = builder.create_asr(add, ConstantInt::get(k, module_));
              instructions.pop_back();
              bb->add_instruction(inst_iter, asr);
              inst->replace_all_use_with(asr);
              deleteInst.insert(inst);
            }
          }
        }
        // 第二个操作数不是常数,第一个是2的n次幂
        casted = ConstantPropagation::cast_to_const_int(value0);
        if (casted && isNthPower(casted->get_value())) {
          LOG(DEBUG) << "inst";
          if (inst->is_mul()) {
            int val = casted->get_value();
            // 0*x
            if (val == 0) {
              inst->replace_all_use_with(ConstantInt::get(0, module_));
              deleteInst.insert(inst);
            } else if (val > 0) {
              int log = 0;
              while (!(val & 1)) {
                val >>= 1;
                log++;
              }
              // 替换为x<<log
              auto res = builder.create_lsl(value1, ConstantInt::get(log, module_));
              instructions.pop_back();
              bb->add_instruction(inst_iter, res);
              inst->replace_all_use_with(res);
              deleteInst.insert(inst);
            }
          }
        }
      }
      for (auto inst: deleteInst) {
        bb->erase_instr(inst);
      }
    }
  }
}