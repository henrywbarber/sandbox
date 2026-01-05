import React from "react";
import { Dimensions, ScrollView, StyleSheet, Text, View } from "react-native";

const { width, height } = Dimensions.get("window");

export default function ExampleScrollable() {
	return (
		<ScrollView
			pagingEnabled
			horizontal
			showsHorizontalScrollIndicator={true}
			style={styles.container}
		>
			<View style={[styles.page, { backgroundColor: "#ffadad" }]}>
				<Text style={styles.text}>Page 1</Text>
			</View>

			<View style={[styles.page, { backgroundColor: "#ffd6a5" }]}>
				<Text style={styles.text}>Page 2</Text>
			</View>

			<View style={[styles.page, { backgroundColor: "#caffbf" }]}>
				<Text style={styles.text}>Page 3</Text>
			</View>
		</ScrollView>
	);
}

const styles = StyleSheet.create({
	container: {
		flex: 1
	},
	page: {
		width: width,
		height: height,
		justifyContent: "center",
		alignItems: "center"
	},
	text: {
		fontSize: 32,
		fontWeight: "bold"
	}
});
