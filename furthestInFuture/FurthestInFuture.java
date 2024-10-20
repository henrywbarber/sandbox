import java.util.*;

// Description: Implementation of Furthest in Future Paging Policy
// Input:
//  int instances
//  int cache size
//  int number of page requests
//  int array deliminated by spaces of cache requests
// Output:
//  Number of page faults
public class FurthestInFuture {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        int numInstances = input.nextInt();
        input.nextLine();

        for (int i = 0; i < numInstances; i++) {
            int cacheSize = input.nextInt();
            input.nextLine();
            input.nextLine();
            String requestLine = input.nextLine();
            String requests[] = requestLine.split(" ");

            // Cache contents
            HashSet<String> cache = new HashSet<>();
            LinkedList<String> cacheList = new LinkedList<>();
            int misses = 0;

            // Store next occurance of a req at index of req
            int[] nextOccurrences = new int[requests.length];
            Map<String, Integer> lastSeen = new HashMap<>();

            // Reverse loop requests 
            for (int r = requests.length - 1; r >= 0; r--) {
                // Check if have an occurance, if not it is the last
                nextOccurrences[r] = lastSeen.getOrDefault(requests[r], -1);
                // Overwrite request next occurance index  
                lastSeen.put(requests[r], r);
            }

            // Loop requests
            for (int r = 0; r < requests.length; r++) {
                // Item not in cache: Miss
                if (!cache.contains(requests[r])) {
                    // Cache full: Replace FF
                    if (cache.size() == cacheSize) {
                        int indexToRemove = -1;
                        int maxFar = -1;

                        // Loop cache items: Indentify furthest cache item to remove
                        for (int c = 0; c < cacheList.size(); c++) {
                            String cachedItem = cacheList.get(c);
                            int nextOccur = lastSeen.getOrDefault(cachedItem, -1);
                            // If next occur of req -1 then auto remove
                            if (nextOccur == -1) {
                                indexToRemove = c;
                                break;
                            }
                            // Update maxFar if occurance is further
                            if (nextOccur > maxFar) {
                                maxFar = nextOccur;
                                indexToRemove = c;
                            }
                        }

                        // Remove and update cache and cacheList
                        cache.remove(cacheList.get(indexToRemove));
                        cacheList.set(indexToRemove, requests[r]);
                        cache.add(requests[r]);
                    } 
                    // Cache has space
                    else {
                        cache.add(requests[r]);
                        cacheList.add(requests[r]);
                    }
                    misses++;
                }
                // Update hashset with requests next occurance index
                lastSeen.put(requests[r], nextOccurrences[r]);
            }
            System.out.println(misses);
        }
        input.close();
    }
}
