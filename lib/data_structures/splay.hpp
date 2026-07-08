#pragma once

#include <bits/stdc++.h>

using namespace std;

struct Node { // Splay tree. Root's pp contains tree's parent.
    Node* p = 0;
    Node* pp = 0; // comment if you don't need it
    array<Node*, 2> c{};

    bool flip = 0;
    int cnt = 1;
    // more fields if wanted

    Node() {
        fix();
    }

    void fix() {
        cnt = 1;
        for (auto& ch : c) {
            if (ch) {
                ch->p = this;
                cnt += ch->cnt;
                // more if wanted
            }
        }
    }

    void pushFlip() {
        if (flip) {
            flip = 0;
            swap(c[0], c[1]);
            if (c[0]) c[0]->flip ^= 1;
            if (c[1]) c[1]->flip ^= 1;
        }
        // more mass updates if wanted
    }

    int up() {
        return p ? p->c[1] == this : -1;
    }

    void rot(int i, int b) {
        int h = i ^ b;
        Node *x = c[i], *y = b == 2 ? x : x->c[h], *z = b ? y : x;
        if ((y->p = p)) p->c[up()] = y;
        c[i] = z->c[i ^ 1];
        if (b < 2) {
            x->c[h] = y->c[h ^ 1];
            y->c[h ^ 1] = x;
        }
        z->c[i ^ 1] = this;
        fix(); x->fix(); y->fix();
        if (p) p->fix();
        swap(pp, y->pp);
    }

    void splay() { // Splay this up to the root. Always finishes without flip set.
        for (pushFlip(); p; ) {
            if (p->p) p->p->pushFlip();
            p->pushFlip(); pushFlip();
            int c1 = up(), c2 = p->up();
            if (c2 == -1) p->rot(c1, 2);
            else p->p->rot(c2, c1 != c2);
        }
    }

    Node* first() { // Return the leftmost element of the subtree rooted at this, splayed to the top.
        pushFlip();
        return c[0] ? c[0]->first() : (splay(), this);
    }
};

enum class side {
    left = 1, // sic
    right = 0,
};

inline Node* merge(Node* le, Node* ri) {
    if (ri == nullptr) return le;
    ri = ri->first();
    ri->c[0] = le;
    ri->fix();
    return ri;
}

// f: Node* -> side (all "left" nodes & their left subtrees will be in left tree)
pair<Node*, Node*> split(Node* v, auto&& f) {
    if (v == nullptr) return {nullptr, nullptr};
    Node* last = nullptr;
    side last_side;
    while (v != nullptr) {
        v->pushFlip();
        last = v;
        last_side = f(v);
        v = v->c[(int)last_side];
    }
    last->splay();
    int i = (int)last_side;
    Node* cut = last->c[i];
    if (cut) cut->p = nullptr;
    last->c[i] = nullptr;
    last->fix();
    if (i) return {last, cut};
    return {cut, last};
}

// example
inline pair<Node*, Node*> split_k(Node* v, int k) {
    return split(v, [k](Node* v) mutable {
        int left_sz = v->c[0] ? v->c[0]->cnt : 0;
        if (left_sz >= k) {
            return side::right;
        }
        k -= left_sz + 1;
        return side::left;
    });
}