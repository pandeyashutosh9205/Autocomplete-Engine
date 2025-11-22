# Autocomplete Engine

A high-performance autocomplete system implemented in C++ featuring three different tree-based search engines: Binary Search Tree (BST), AVL Tree, and Red-Black Tree.


## 🎯 Features

- **Real-time Autocomplete**: Type-ahead search suggestions as you type
- **Multiple Search Engines**: Switch between BST, AVL, and Red-Black Tree implementations
- **Search History**: Automatically saves and loads your search history
- **Performance Benchmarking**: Compare the performance of different tree structures
- **Case-Insensitive Search**: Find matches regardless of letter casing
- **Interactive UI**: Clean console-based interface with keyboard navigation

## 🏗️ Architecture

### Data Structures Implemented

1. **Binary Search Tree (BST)**
   - Simple implementation
   - Fast for random data
   - Degrades to O(n) with sorted data

2. **AVL Tree**
   - Self-balancing tree
   - Guarantees O(log n) operations
   - Stricter balancing (more rotations)

3. **Red-Black Tree**
   - Self-balancing with relaxed constraints
   - Faster insertions than AVL
   - Still maintains O(log n) complexity

4. **Linked List**
   - Used for search history management
   - Simple LIFO structure

## 📋 Requirements

- C++ Compiler (supporting C++11 or later)
- Windows OS (uses `conio.h` for keyboard input)
- `data.txt` file containing your dataset (one entry per line)

## 🚀 Getting Started

### Installation

1. Clone or download the project files
2. Create a `data.txt` file in the same directory with your data:
   ```
   Apple Inc.
   Microsoft Corporation
   Google LLC
   Amazon.com
   Facebook Inc.
   ...
   ```

3. Compile the program:
   ```bash
   g++ -o autocomplete main.cpp -std=c++11
   ```

4. Run the executable:
   ```bash
   ./autocomplete
   ```

## 🎮 Usage

### Main Menu Options

1. **Search Directory**: Enter the interactive autocomplete interface
2. **View Recent Search History**: Display previously searched terms
3. **Display All Data**: Show all entries in alphabetical order
4. **Run Performance Benchmark**: Compare tree performance on sorted data
5. **Switch Search Engine**: Toggle between BST, AVL, and RB Tree
6. **Exit**: Save history and close the application

### Search Interface

- **Type**: Start typing to see autocomplete suggestions (up to 5 matches)
- **Backspace**: Delete characters
- **Enter**: Select the current search term
- **Esc**: Return to main menu

## 📊 Performance Characteristics

| Tree Type | Insert (Random) | Insert (Sorted) | Search (Balanced) | Search (Worst) |
|-----------|----------------|-----------------|-------------------|----------------|
| BST       | O(log n)       | O(n)            | O(log n)          | O(n)           |
| AVL       | O(log n)       | O(log n)        | O(log n)          | O(log n)       |
| Red-Black | O(log n)       | O(log n)        | O(log n)          | O(log n)       |

## 🔍 How It Works

1. **Data Loading**: Reads entries from `data.txt` into memory
2. **Tree Building**: Constructs all three tree structures simultaneously
3. **Autocomplete Search**: 
   - Traverses the tree based on prefix matching
   - Uses case-insensitive comparison
   - Returns up to 5 matching results
4. **History Management**: 
   - Stores searches in a linked list
   - Persists to `log.txt` on exit
   - Loads previous history on startup

## 📁 File Structure

```
.
├── main.cpp           # Main program file
├── data.txt           # Input dataset (create this)
├── log.txt            # Search history (auto-generated)
└── README.md          # This file
```

## 🎨 Key Implementation Details

### Autocomplete Algorithm
- Performs prefix matching with case-insensitive comparison
- Limits results to 5 suggestions for optimal UX
- Recursively traverses tree in lexicographic order

### Tree Balancing
- **AVL**: Strict height-balanced (height difference ≤ 1)
- **Red-Black**: Color-based balancing with specific properties
- Both maintain O(log n) height guarantee

### History Management
- Uses stack for correct chronological ordering
- Saves in reverse to maintain proper sequence
- Prevents duplicate entries in trees

## 🐛 Known Limitations

- Windows-only (uses `conio.h` for keyboard handling)
- Maximum 5 autocomplete suggestions
- No fuzzy matching (exact prefix match only)
- Terminal-based UI only

## 🔮 Future Enhancements

- Cross-platform support (Linux/Mac)
- Fuzzy search algorithm
- Configurable suggestion limit
- GUI implementation
- Support for Unicode characters
- Search result ranking

## 📝 License

This project was created as an academic assignment. Feel free to use and modify for educational purposes.

## 🤝 Contributing

This is an academic project, but suggestions and improvements are welcome! Feel free to fork and experiment with different implementations.

---

**Note**: Make sure your `data.txt` file contains meaningful data for the autocomplete to work effectively. The more entries you have, the better the performance comparison will be!
