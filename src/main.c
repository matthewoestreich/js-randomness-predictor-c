#include <stdio.h>
#include <z3.h>

int main(void) {
  Z3_config cfg = Z3_mk_config();
  Z3_context ctx = Z3_mk_context(cfg);
  Z3_del_config(cfg);

  Z3_sort int_sort = Z3_mk_int_sort(ctx);
  Z3_symbol x_sym = Z3_mk_string_symbol(ctx, "x");
  Z3_ast x = Z3_mk_const(ctx, x_sym, int_sort);

  Z3_ast one = Z3_mk_int(ctx, 1, int_sort);
  Z3_ast x_plus_1 = Z3_mk_add(ctx, 2, (Z3_ast[]){x, one});
  Z3_ast prop = Z3_mk_gt(ctx, x_plus_1, x);  // x + 1 > x

  Z3_solver solver = Z3_mk_solver(ctx);
  Z3_solver_inc_ref(ctx, solver);

  Z3_ast not_prop = Z3_mk_not(ctx, prop);
  Z3_solver_assert(ctx, solver, not_prop);

  printf("Checking if x + 1 > x is always true...\n");
  Z3_lbool result = Z3_solver_check(ctx, solver);

  if (result == Z3_L_FALSE) {
    printf("VALID: x + 1 > x is always true\n");
    Z3_del_context(ctx);
    return 1;
  }

  if (result == Z3_L_TRUE) {
    printf("INVALID: Found a counterexample\n");
    Z3_model model = Z3_solver_get_model(ctx, solver);
    if (model) {
      Z3_string mstr = Z3_model_to_string(ctx, model);
      printf("Model:\n%s\n", mstr);
    } else {
      printf("Model is NULL\n");
    }
    Z3_del_context(ctx);
    return 0;
  }

  printf("UNKNOWN result\n");
  Z3_del_context(ctx);
  return 0;
}
