/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.ther;

import com.google.common.base.MoreObjects;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.block.BlockBorder;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.renderer.xy.*;
import org.jfree.data.xy.XYDataset;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;

/**
 * This one is meant only as exercise, it regulates fluid level in a cylindrical tank.
 * Water is flowing into a tank and it' coming out. As a result water level is changing.
 */
public class FluidLevelTest {
    private final Output out = new Output();
    private final Driver driver;


    /**
     * Diameter of the tank.
     */
    private static final float DIAMETER = 3;

    private static final int PROBES = 20;

    public static void main(String... args) {

        FluidLevelTest ex = new FluidLevelTest(new PIDDriver());
        ex.calculate();
    }


    public void calculate() {
        Output setPoint = new Output();
        setPoint.level = 1;

        Input in = new Input();
        in.flowIn = 1;

        List<ProbePoint> probes = new ArrayList<>(PROBES);
        for (int sample = 0; sample < PROBES; sample++) {
            Output out = probe(in);
            driver.drive(setPoint, out, in);

            System.out.println(in + " " + out);
            probes.add(new ProbePoint(in, out, setPoint, sample));
        }

        ChartUI chartUI = new ChartUI();
        chartUI.createUI(probes);
        chartUI.setVisible(true);
    }

    public FluidLevelTest(Driver driver) {
        this.driver = driver;


    }

    public Output probe(Input in) {
        out.level += (in.flowIn + in.flowOut) / DIAMETER;
        if (out.level < 0) {
            out.level = 0;
        }
        return out;
    }


    interface Driver {
        void drive(Output setPoint, Output current, Input in);
    }

    static class PIDDriver implements Driver {
        private static final float AMP_P = 10;
        private static final float AMP_I = 0;
        private static final float AMP_D = 0;

        @Override
        public void drive(Output setPoint, Output current, Input in) {
            float valP = calculateP(setPoint, current);

            float pid = valP;
            if (pid > 0) {
                pid = 0;
            }

            in.flowOut = pid;
        }

        private float calculateP(Output setPoint, Output current) {
            float derivation = setPoint.level - current.level;
            return AMP_P * derivation;
        }
    }

    static class ProbePoint {
        Output out;
        Input in;
        Output setPoint;
        int sample;

        public ProbePoint(Input in, Output out, Output setPoint, int sample) {
            this.in = in.clone();
            this.out = out.clone();
            this.setPoint = setPoint.clone();
            this.sample = sample;
        }

    }

    static class Input implements Cloneable {
        /**
         * water flowing into the tank.
         */
        float flowIn = 0;

        /**
         * water leaving the tank.
         */
        float flowOut = 0;


        public Input(float flowIn, float flowOut) {
            this.flowIn = flowIn;
            this.flowOut = flowOut;
        }

        public Input() {
        }

        @Override
        protected Input clone() {
            return new Input(flowIn, flowOut);
        }

        @Override
        public String toString() {
            return MoreObjects.toStringHelper("IN")
                    .add("flowIn", flowIn)
                    .add("flowOut", flowOut)
                    .toString();
        }
    }

    static class Output implements Cloneable {

        /**
         * water level (depth) in the tank (h).
         */
        float level = 0;

        public Output(float level) {
            this.level = level;
        }

        public Output() {
        }

        @Override
        protected Output clone() {
            return new Output(level);
        }

        @Override
        public String toString() {
            return MoreObjects.toStringHelper("OUT")
                    .add("level", level)
                    .toString();
        }
    }

    static class ChartUI extends JFrame {

        private final static String TITLE = "Fluid level change in time";

        private XYDataset createDataset(List<ProbePoint> probes) {
            XYSeries seriesIn = new XYSeries("in");
            XYSeries seriesOut = new XYSeries("out");
            XYSeries seriesLevel = new XYSeries("level");
            XYSeries seriesSp = new XYSeries("sp");

            for (ProbePoint point : probes) {
                int sample = point.sample;
                seriesIn.add(sample, point.in.flowIn);
                seriesOut.add(sample, point.in.flowOut);
                seriesLevel.add(sample, point.out.level);
                seriesSp.add(sample, point.setPoint.level);
            }
            XYSeriesCollection dataset = new XYSeriesCollection();
            dataset.addSeries(seriesIn);
            dataset.addSeries(seriesOut);
            dataset.addSeries(seriesLevel);
            dataset.addSeries(seriesSp);

            return dataset;
        }

        private JFreeChart createChart(XYDataset dataset) {

            JFreeChart chart = ChartFactory.createXYLineChart(
                    TITLE,
                    "Time",
                    "Level",
                    dataset,
                    PlotOrientation.VERTICAL,
                    true,
                    false,
                    false
            );

            AbstractXYItemRenderer renderer = new CyclicXYItemRenderer();

            renderer.setSeriesPaint(0, Color.RED);
            renderer.setSeriesStroke(0, createStroke());

            renderer.setSeriesPaint(1, Color.BLUE);
            renderer.setSeriesStroke(1, createStroke());

            renderer.setSeriesPaint(2, Color.GREEN);
            renderer.setSeriesStroke(2, createStroke());

            renderer.setSeriesPaint(3, Color.PINK);
            renderer.setSeriesStroke(3, createStroke());

            XYPlot plot = chart.getXYPlot();
            plot.setRenderer(renderer);
            plot.setBackgroundPaint(Color.GRAY);

            chart.getLegend().setFrame(BlockBorder.NONE);
            return chart;
        }

        private Stroke createStroke() {
            return new BasicStroke(2.0f);
        }

        private void createUI(List<ProbePoint> probes) {
            XYDataset dataset = createDataset(probes);
            JFreeChart chart = createChart(dataset);

            ChartPanel chartPanel = new ChartPanel(chart);
            chartPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
            chartPanel.setBackground(Color.BLACK);
            add(chartPanel);

            pack();
            setTitle(TITLE);
            setLocationRelativeTo(null);
            setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        }
    }
}
