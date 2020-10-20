#ifndef SRC_ADVENTURE_H_
#define SRC_ADVENTURE_H_

#include <algorithm>
#include <queue>
#include <vector>
#include "../third_party/threadpool/threadpool.h"

#include "./types.h"
#include "./utils.h"

class Adventure {
 public:
  virtual ~Adventure() = default;

  virtual uint64_t packEggs(std::vector<Egg> eggs, BottomlessBag& bag) = 0;

  virtual void arrangeSand(std::vector<GrainOfSand>& grains) = 0;

  virtual Crystal selectBestCrystal(std::vector<Crystal>& crystals) = 0;

 protected:
  uint64_t max(uint64_t a, uint64_t b) { return (a > b) ? a : b; }

  void merge(std::vector<GrainOfSand>& grains, size_t left, size_t m,
             size_t right) {
    size_t i, j, k;
    size_t n1 = m - left + 1;
    size_t n2 = right - m;

    std::vector<GrainOfSand> L(n1), R(n2);

    for (i = 0; i < n1; i++) L[i] = grains[left + i];
    for (j = 0; j < n2; j++) R[j] = grains[m + 1 + j];

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2) {
      if (R[j] < L[i]) {
        grains[k] = R[j];
        j++;
      } else {
        grains[k] = L[i];
        i++;
      }
      k++;
    }

    while (i < n1) {
      grains[k] = L[i];
      i++;
      k++;
    }

    while (j < n2) {
      grains[k] = R[j];
      j++;
      k++;
    }
  }
};

class LonesomeAdventure : public Adventure {
 public:
  LonesomeAdventure() {}

  virtual uint64_t packEggs(std::vector<Egg> eggs, BottomlessBag& bag) {
    uint64_t W = bag.getCapacity();
    uint64_t n = eggs.size();

    std::vector<std::vector<uint64_t>> m(n + 1,
                                         std::vector<uint64_t>(W + 1, 0));

    for (uint64_t jw = 0; jw <= W; jw++) m[0][jw] = 0;

    for (uint64_t i = 1; i <= n; i++) {
      for (uint64_t jw = 0; jw <= W; jw++) {
        if (eggs[i - 1].getSize() > jw)
          m[i][jw] = m[i - 1][jw];
        else
          m[i][jw] =
              max(m[i - 1][jw], eggs[i - 1].getWeight() +
                                    m[i - 1][jw - eggs[i - 1].getSize()]);
      }
    }
    return m[n][W];
  }

  virtual void arrangeSand(std::vector<GrainOfSand>& grains) {
    size_t n = grains.size();
    size_t currentSize = 2;
    size_t left = 0;

    while (currentSize <= 2 * n) {
      while (left + currentSize - 1 < n) {
        merge(grains, left, left + (currentSize - 1) / 2,
              left + currentSize - 1);
        left += currentSize;
      }
      if (left + (currentSize - 1) / 2 < n) {
        merge(grains, left, left + (currentSize - 1) / 2, n - 1);
      }

      left = 0;
      currentSize *= 2;
    }
  }

  virtual Crystal selectBestCrystal(std::vector<Crystal>& crystals) {
    Crystal max = Crystal();

    for (auto itr = crystals.begin(); itr != crystals.end(); ++itr) {
      if (max < *itr) {
        max = *itr;
      }
    }

    return max;
  }
};

class TeamAdventure : public Adventure {
 public:
  explicit TeamAdventure(uint64_t numberOfShamansArg)
      : numberOfShamans(numberOfShamansArg),
        councilOfShamans(numberOfShamansArg) {}

  uint64_t packEggs(std::vector<Egg> eggs, BottomlessBag& bag) {
    uint64_t W = bag.getCapacity();
    uint64_t n = eggs.size();
    uint64_t** m = new uint64_t*[n + 1];

    for (size_t i = 0; i <= n; i++) {
      m[i] = new uint64_t[W + 1];
      for (size_t j = 0; j < W + 1; j++) m[i][j] = 0;
    }

    std::queue<std::future<void>> tasks;

    for (uint64_t i = 1; i <= n; i++) {
      size_t size = (W + 1) / numberOfShamans;
      size_t left = 0;

      if (size > 0) {
        while (left + size <= W + 1) {
          tasks.push(councilOfShamans.enqueue(&TeamAdventure::knapsack, this, m,
                                              eggs[i - 1], i, left,
                                              left + size));
          left += size;
        }
      }

      tasks.push(councilOfShamans.enqueue(&TeamAdventure::knapsack, this, m,
                                          eggs[i - 1], i, left, W + 1));

      while (!tasks.empty()) {
        tasks.front().get();
        tasks.pop();
      }
    }

    uint64_t result = m[n][W];

    for (size_t i = 0; i <= n; i++) {
      delete[] m[i];
    }
    delete[] m;

    return result;
  }

  virtual void arrangeSand(std::vector<GrainOfSand>& grains) {
    const size_t minSize = 8;
    size_t n = grains.size();
    size_t currentSize = 2;
    size_t left = 0;

    while (currentSize <= 2 * n) {
      if (currentSize > minSize) {
        std::queue<std::future<void>> tasks;

        while (left + currentSize - 1 < n) {
          tasks.push(councilOfShamans.enqueue(
              &TeamAdventure::merge, this, std::ref(grains), left,
              left + (currentSize - 1) / 2, left + currentSize - 1));

          left += currentSize;
        }
        if (left + (currentSize - 1) / 2 < n) {
          tasks.push(councilOfShamans.enqueue(
              &TeamAdventure::merge, this, std::ref(grains), left,
              left + (currentSize - 1) / 2, n - 1));
        }

        while (!tasks.empty()) {
          tasks.front().get();
          tasks.pop();
        }
      } else {
        while (left + currentSize - 1 < n) {
          merge(grains, left, left + (currentSize - 1) / 2,
                left + currentSize - 1);
          left += currentSize;
        }
        if (left + (currentSize - 1) / 2 < n) {
          merge(grains, left, left + (currentSize - 1) / 2, n - 1);
        }
      }

      left = 0;
      currentSize *= 2;
    }
  }

  virtual Crystal selectBestCrystal(std::vector<Crystal>& crystals) {
    std::queue<std::future<Crystal>> tasks;
    const size_t n = crystals.size();
    size_t size = n / numberOfShamans;
    size_t left = 0;

    for (uint64_t i = 1; i < numberOfShamans; i++) {
      tasks.push(councilOfShamans.enqueue(&TeamAdventure::intervalMax, this,
                                          crystals, left, left + size));

      left += size;
    }

    tasks.push(councilOfShamans.enqueue(&TeamAdventure::intervalMax, this,
                                        crystals, left, n));

    Crystal maxCrystal(0);

    while (!tasks.empty()) {
      Crystal best = tasks.front().get();
      if (maxCrystal < best) maxCrystal = best;

      tasks.pop();
    }

    return maxCrystal;
  }

 private:
  uint64_t numberOfShamans;
  ThreadPool councilOfShamans;

  Crystal intervalMax(std::vector<Crystal>& crystals, size_t begin,
                      size_t end) {
    Crystal maxCrystal(0);

    for (size_t i = begin; i < end; i++) {
      if (maxCrystal < crystals[i]) maxCrystal = crystals[i];
    }

    return maxCrystal;
  }

  void knapsack(uint64_t** m, Egg& egg, uint64_t i, uint64_t left,
                uint64_t right) {
    for (uint64_t jw = left; jw < right; jw++) {
      if (egg.getSize() > jw)
        m[i][jw] = m[i - 1][jw];
      else
        m[i][jw] =
            max(m[i - 1][jw], egg.getWeight() + m[i - 1][jw - egg.getSize()]);
    }
  }
};

#endif  // SRC_ADVENTURE_H_
