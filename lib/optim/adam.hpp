#ifndef CODE_FOR_REUSE_OPTIM_ADAM_HPP_
#define CODE_FOR_REUSE_OPTIM_ADAM_HPP_

#include <vector>

template <class ld = long double, ld epsilon = (ld)1e-8, ld fast_lr_decay = (ld)0.9, ld beta1 = (ld)0.9,
          ld beta2 = (ld)0.999>
struct Adam {
    static_assert(fast_lr_decay > 0 && fast_lr_decay <= 1);
    static_assert(beta1 > 0 && beta1 < 1);
    static_assert(beta2 > 0 && beta2 < 1);

    // in theory, lr should be ~||x_opt - x_0||, but you should be able set it a bit higher or lower
    explicit Adam(int n, ld lr, ld lr_decay = 1)
        : lr(lr), lr_decay(lr_decay), variables(n), impulse(n), momentum(n), best_variables(n) {
    }

    static Adam auto_decay(int n, int iter, ld lr) {
        return Adam(n, lr, exp((log(epsilon) - log(lr)) / iter));
    }

    // use it in optimization loop like 'for (...; adam && ...; )' to speed up program
    explicit operator bool() const {
        return lr > epsilon;
    }

    void step(ld loss, const auto& grad) {
        if (isnan(loss) || isinf(loss) || loss > min_loss) {
            // rollback
            variables = best_variables;

            // heuristics to prevent further misses (vary them if you want)
            lr *= fast_lr_decay;
            for (int i = 0; i < variables.size(); ++i) {
                momentum[i] /= 2;
                impulse[i] /= 2;
            }
            beta1_power = 1;
            beta2_power = 1;
            return;
        }
        min_loss = loss;
        best_variables = variables;
        beta1_power *= beta1;
        beta2_power *= beta2;
        for (int i = 0; i < variables.size(); ++i) {
            impulse[i] = impulse[i] * beta1 + (1 - beta1) * grad[i];
            momentum[i] = momentum[i] * beta2 + (1 - beta2) * grad[i] * grad[i];
            ld impulse_corr = impulse[i] / (1 - beta1_power);
            ld momentum_corr = momentum[i] / (1 - beta2_power);
            variables[i] -= lr / (sqrt(momentum_corr) + epsilon) * impulse_corr;
        }
        lr *= lr_decay;
    }

    ld lr;
    ld lr_decay;

    ld beta1_power = 1;
    ld beta2_power = 1;

    std::vector<ld> variables;
    std::vector<ld> impulse;
    std::vector<ld> momentum;

    ld min_loss = 1e100;
    std::vector<ld> best_variables;
};

#endif  // CODE_FOR_REUSE_OPTIM_ADAM_HPP_